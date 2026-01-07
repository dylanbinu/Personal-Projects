import sys
import os
import json
import asyncio
import nest_asyncio
import re
from urllib.parse import urlparse, urlunparse
from playwright.async_api import async_playwright
from bs4 import BeautifulSoup 

nest_asyncio.apply()

# --- CONFIGURATION ---
MAX_PAGES_TO_CRAWL = 1000
TIMEOUT_MS = 15000 
CONCURRENT_BATCH_SIZE = 15

# --- SEED EXPANSIONS ---
# Automatically try these paths to ensure we hit critical pages even if nav links are hidden
SEED_EXPANSIONS = [
    "/locations", "/campuses", "/visit", "/times", "/connect", "/about", "/new", "/give", "/giving", "/donate"
]



# Junk to ignore
IGNORE_KEYWORDS = [
    "login", "signin", "signup", "register", "cart", "checkout", "auth",
    "account", "password", "reset", "privacy", "terms", "policy", 
    "facebook", "twitter", "instagram", "linkedin", "tiktok", "share",
    "mailto:", "javascript:", "#", "unsubscribe", "feed", "rss"
]

# Expanded junk list to aggressively clean "Modern Web" noise
BOILERPLATE_PHRASES = [
    # Cookie / GDPR / Legal
    "manage consent", "preferences", "rights reserved", "basic website functionality",
    "deliver advertising", "cookie policy", "privacy policy", "terms of use",
    "accept all", "reject all", "save preferences", "always active", "checkbox",
    "marketing", "analytics", "personalization", "essential",
    "view our privacy policy", "stored or retrieved", "impact your experience",
    "data in your browser", "cancel", "decline", "accept",
    
    # Navigation / UI Noise
    "skip to content", "view map", "get directions", "menu", "search",
    "log in", "sign up", "cart", "checkout", "close", "open", "toggle",
    "share event", "register", "learn more", "watch now", "give", "i'm new",
    "watch replay", "attend online", "watch live in", "00h : 00m", # Countdowns
    
    # Copyright / Footer
    "ccli", "streaming license", "copyright", "all rights reserved",
    "powered by", "site by", "website by"
]

def clean_url(url):
    parsed = urlparse(url)
    clean = urlunparse((parsed.scheme, parsed.netloc, parsed.path, "", "", ""))
    return clean.rstrip("/")

def clean_extracted_text(text, url=""):
    if not text: return ""
    lines = text.split('\n')
    cleaned_lines = []
    
    # Check if this is a "Locations" page where we WANT the locations list
    is_location_page = False
    if url:
        u = url.lower()
        if "/locations" in u or "/campuses" in u or "/visit" in u or "/contact" in u:
            is_location_page = True

    for line in lines:
        line = line.strip()
        line_lower = line.lower()
        
        # 1. Skip Empty or Tiny lines (unless it looks like a time/date)
        if len(line) < 3:
            continue
            
        # 2. Skip Boilerplate/Junk
        if any(phrase in line_lower for phrase in BOILERPLATE_PHRASES):
            continue
            
        # 3. Skip "Watch live" countdowns specific to this site format
        if ":" in line and "d :" in line: # Matches "6d : 02h : 15m" type patterns
            continue
            
        # 4. Skip isolated UI words that appear frequently
        if line_lower in ["give", "i'm new", "kids", "youth", "adults", "families", "classes", "events", "featured", "view all", "share event"]:
            continue
            
        cleaned_lines.append(line)
    
    # Dedup lines (fixes repeated footer addresses/service times)
    seen = set()
    deduped = []
    for line in cleaned_lines:
        if line not in seen:
            deduped.append(line)
            seen.add(line)

    # Detect and strip repetitive footer (## Locations followed by campus list)
    final_lines = []
    footer_detected = False
    
    for i, line in enumerate(deduped):
        # Check for Footer Start Identifier
        if "## Locations" in line:
            # Look ahead to see if it matches a generic footer pattern
            # (Usually followed by an address or phone number pattern, or just very repetitive links)
            is_generic_footer = False
            
            # Simple Heuristic: If we see "Campus" or "Location" repeated in the next few lines
            # it is likely the footer list.
            for forward_line in deduped[i+1:i+6]: # Check next 5 lines
                lower_f = forward_line.lower()
                if "campus" in lower_f or "location" in lower_f or "service time" in lower_f:
                    is_generic_footer = True
                    break
            
            # CRITICAL FIX: Only strip if it's a footer AND NOT the main content of a location page
            if is_generic_footer and not is_location_page:
                footer_detected = True
                break # Stop processing lines (Truncate the rest)
        
        final_lines.append(line)

    return "\n".join(final_lines)

def is_useful_link(base_url, link):
    if not link or not link.startswith("http"): return False
    if urlparse(base_url).netloc != urlparse(link).netloc: return False
    
    link_lower = link.lower()
    if link_lower.endswith(('.pdf', '.jpg', '.png', '.css', '.js', '.zip', '.xml', '.mp3', '.mp4')): return False
    if any(bad in link_lower for bad in IGNORE_KEYWORDS): return False
    
    return True

async def process_batch(context, batch_urls):
    tasks = []
    # Resource blocking is now handled globally on the context passed in
    for url in batch_urls:
        tasks.append(scrape_single_page(context, url))
    
    results = await asyncio.gather(*tasks)
    return results

async def scrape_single_page(context, url):
    print(f"   ...Crawling: {url}")
    try:
        page = await context.new_page()
        await page.goto(url, wait_until="domcontentloaded", timeout=TIMEOUT_MS)
        
        html = await page.content()
        soup = BeautifulSoup(html, "html.parser")
        
        for script in soup(["script", "style", "nav", "footer", "header"]):
            script.extract()
            
        # raw_text = soup.get_text(separator="\n")
        # clean_text = clean_extracted_text(raw_text)

        # --- UPDATED: CONVERT TO MARKDOWN ---
        import html2text
        h = html2text.HTML2Text()
        h.ignore_links = False
        h.ignore_images = True
        h.ignore_emphasis = False  # Keep bold/italic
        h.skip_internal_links = True
        h.body_width = 0  # Disable line wrapping
        
        # Configure table support (html2text handles simple tables automatically)
        
        # Convert HTML -> Markdown
        markdown_text = h.handle(html)
        
        # Post-process Markdown to remove excessive noise
        clean_text = clean_extracted_text(markdown_text)
        
        links = await page.eval_on_selector_all("a[href]", "elements => elements.map(e => e.href)")
        await page.close()
        return {"url": url, "text": clean_text, "links": links}
        
    except Exception as e:
        return {"url": url, "text": "", "links": []}

async def recursive_scrape(seed_url):
    results = [] 
    print(f"Starting Church Scrape on: {seed_url}")
    
    async with async_playwright() as p:
        browser = await p.chromium.launch(headless=True)
        # Create a single context for the entire scrape session
        context = await browser.new_context(
            user_agent="Mozilla/5.0 (Windows NT 10.0; Win64; x64) Chrome/91.0.4472.124 Safari/537.36"
        )
        # Block heavy resources globally for this context
        await context.route("**/*.{png,jpg,jpeg,gif,webp,svg,ico,mp4,avi,mov,css,woff,woff2}", lambda route: route.abort())

        visited = set()
        # Queue is now just a list of URLs (BFS Traversal)
        queue = [seed_url]
        
        # Add Seed Expansions
        parsed = urlparse(seed_url)
        base = f"{parsed.scheme}://{parsed.netloc}"
        for ext in SEED_EXPANSIONS:
            full = base + ext
            queue.append(full)
            
        pages_scraped = 0
        
        while queue and pages_scraped < MAX_PAGES_TO_CRAWL:
            batch_urls = []
            
            # Pop from front (BFS)
            while len(batch_urls) < CONCURRENT_BATCH_SIZE and queue:
                url = queue.pop(0)
                clean = clean_url(url)
                if clean in visited: continue
                visited.add(clean)
                batch_urls.append(url)
            
            if not batch_urls: break
                
            print(f"   --- Batch {pages_scraped // CONCURRENT_BATCH_SIZE + 1} ---")
            batch_results = await process_batch(context, batch_urls)
            
            for res in batch_results:
                pages_scraped += 1
                text = res['text']
                found_links = res['links']
                current_url = res['url']
                
                if text and len(text) > 50:
                    clean_text = clean_extracted_text(text, current_url)
                    results.append({"source": current_url, "content": clean_text})
                
                for link in found_links:
                    c_link = clean_url(link)
                    if is_useful_link(seed_url, link) and c_link not in visited:
                        # OPTIMIZATION: Queue the CLEANED link to strictly prevent query-param duplicates
                        # This avoids "Calendar Traps" (e.g. ?date=2025-01, ?date=2025-02)
                        # We only crawl the canonical page.
                        if c_link not in queue:
                            queue.append(c_link)

        await context.close()
        await browser.close()
    return results

def main():
    target_url = None
    output_file = "scraped_data.jsonl" # Default

    # Simple arg parsing
    args = sys.argv[1:]
    if len(args) >= 1:
        target_url = args[0]
    if len(args) >= 2:
        output_file = args[1] 

    if not target_url:
        target_url = input("Enter Church URL: ").strip()
    
    if not target_url.startswith("http"): target_url = "https://" + target_url

    data_points = asyncio.run(recursive_scrape(target_url))
    
    if not data_points:
        print("❌ No data found.")
        return

    base_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.join(base_dir, "..")
    
    # If output_file is just a name, save in project root. If path, use as is.
    if os.path.isabs(output_file):
        filename = output_file
    else:
        filename = os.path.join(project_root, output_file)
    
    if os.path.exists(filename): os.remove(filename)
    
    with open(filename, "w", encoding="utf-8") as f:
        for entry in data_points:
            f.write(json.dumps(entry) + "\n")
            
    print(f"Saved {len(data_points)} pages to {filename}")

if __name__ == "__main__":
    main()