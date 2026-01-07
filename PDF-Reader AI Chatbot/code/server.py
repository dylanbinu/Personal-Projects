import os
import json
import re
from dotenv import load_dotenv

load_dotenv()

from typing import List
from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from langchain_chroma import Chroma
from langchain_huggingface import HuggingFaceEmbeddings
from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.messages import HumanMessage, AIMessage
from retrieval import retrieve_and_rank
from typing import Optional

# --- CONFIGURATION (Copied from app.py) ---
if not os.environ.get("OPENAI_API_KEY"):
    raise RuntimeError("OPENAI_API_KEY not found in environment. Please check your .env file.")

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.join(BASE_DIR, "..")
CHROMA_PATH = os.path.join(PROJECT_ROOT, "chroma_db")
CHURCHES_FILE = os.path.join(BASE_DIR, "churches.json")
DEFAULT_DATA_FILE = os.path.join(PROJECT_ROOT, "scraped_data.jsonl")

# --- LOAD CHURCH REGISTRY & CONTEXT ---
CHURCHES = {}
CHURCH_CONTEXTS = {} # Map church_id -> {valid_urls: set, main_domain: str, context_keyword: str}

if os.path.exists(CHURCHES_FILE):
    with open(CHURCHES_FILE, "r") as f:
        CHURCHES = json.load(f)
    print(f"Loaded {len(CHURCHES)} churches from registry.")

def load_context_from_file(filepath):
    valid_urls = set()
    main_domain = ""
    preferred_keyword = None
    
    if not os.path.exists(filepath):
        return None

    try:
        from collections import Counter
        from urllib.parse import urlparse
        
        with open(filepath, "r", encoding="utf-8") as f:
            # Read first line for keyword
            first_line = f.readline()
            if first_line:
                data = json.loads(first_line)
                first_url = data.get("source", "").strip("/")
                if "/" in first_url:
                    candidate = first_url.split("/")[-1]
                    if candidate.lower() not in ["locations", "campuses", "visit", "home", "en", "welcome"] and "." not in candidate:
                        preferred_keyword = candidate.lower()

            f.seek(0)
            for line in f:
                try:
                    data = json.loads(line)
                    url = data.get("source")
                    content = data.get("content", "").lower()
                    if "page not found" in content or "404" in content: continue
                    if url: valid_urls.add(url)
                except: pass
        
        if valid_urls:
            domains = [urlparse(u).netloc for u in valid_urls]
            common = Counter(domains).most_common(1)
            if common:
                main_domain = f"https://{common[0][0]}"
                
        return {
            "valid_urls": valid_urls,
            "main_domain": main_domain,
            "preferred_keyword": preferred_keyword
        }
    except Exception as e:
        print(f"Error loading context from {filepath}: {e}")
        return None

# Load Default (Legacy) Context
DEFAULT_CONTEXT = load_context_from_file(DEFAULT_DATA_FILE)
if not DEFAULT_CONTEXT:
    DEFAULT_CONTEXT = {"valid_urls": set(), "main_domain": "https://google.com", "preferred_keyword": None}

# Load Specific Church Contexts (if data files exist)
# Expectation: scraped_data_{church_id}.jsonl
for church_id in CHURCHES:
    path = os.path.join(PROJECT_ROOT, f"scraped_data_{church_id}.jsonl")
    ctx = load_context_from_file(path)
    if ctx:
        CHURCH_CONTEXTS[church_id] = ctx
        print(f"Loaded context for {church_id}")
    else:
        # Fallback to default if no specific file (Shared data model?)
        # Or just don't load context.
        pass


# Helper: Common hallucinations mapped to real paths (partial or full)
KEYWORD_FALLBACKS = {
    "give": ["give", "giving", "donate", "donation", "tithe", "offering"],
    "giving": ["give", "giving", "donate", "donation", "tithe", "offering"],
    "prayer": ["care", "prayer", "request"],
    "watch": ["watch", "message", "sermon", "media", "youtube"],
    "location": ["contact", "visit", "about", "home"],
    "locations": ["contact", "visit", "about", "home"],
    "kids": ["kid", "child", "family", "nursery"],
    "youth": ["youth", "student", "teen"]
}

def validate_and_fix_links(text: str, valid_urls: set, fallback_url: str, main_domain: str = "") -> str:
    """
    Parses markdown links [text](url).
    Checks if 'url' is in valid_urls.
    If not, tries to find a close match or fallback.
    """
    def replace_link(match):

        label = match.group(1)
        url = match.group(2).rstrip("/")
        label_lower = label.lower()

        # --- 0. LABEL-BASED OVERRIDES (Fix Hallucinations/Bad Deep Links) ---
        
        # Calculate roots once (safe scope)
        roots = [v for v in valid_urls if v.rstrip('/').count('/') == 2]
        
        # A. "Home Page" -> Force Root Domain
        if "home page" in label_lower and "contact" not in label_lower:
            if roots:
                return f"[{label}]({min(roots, key=len)})"

        # B. "Plan Your Visit" -> specific page OR Root
        if "plan your visit" in label_lower or "visit us" in label_lower:
            # Try to find a REALLY good match first
            visits = [v for v in valid_urls if "/plan-your-visit" in v or "/visit" in v or "/im-new" in v]
            best_visit = None
            # Filter out known bad patterns if possible, or just pick shortest
            if visits:
                 # heuristic: pick shortest to avoid specific blog posts "visit-to-orphanage"
                 best_visit = min(visits, key=len)
            
            if best_visit:
                 return f"[{label}]({best_visit})"
            elif roots:
                 # Fallback to Home Page if no specific visit page
                 return f"[{label}]({min(roots, key=len)})"

        # 1. Exact Match
        if url in valid_urls:
            return f"[{label}]({url})"
        
        # 2. Case Insensitive Match
        for v in valid_urls:
            if v.lower() == url.lower():
                return f"[{label}]({v})"
            
            # Dynamic Partial Match: Use main_domain if available
            if main_domain:
                # e.g. url was "/contact", v is "https://church.com/contact"
                # Check if url is a suffix of v
                if url.startswith("/") and v.endswith(url):
                     return f"[{label}]({v})"
                 
                # Check if stripped URL is in v
                # e.g. url="https://wrong-domain.com/contact", v="https://church.com/contact"
                # This is tricky. Let's stick to relative path matching.
                if url.replace(main_domain, "") in v and len(url) > 10:
                     return f"[{label}]({v})"

        # 3. Smart Keyword Fallback (Generic)
        # Check if the hallucinates URL path (e.g. "give") matches a known concept
        cleaned_path = url.strip("/").lower()
        
        # Check our predefined concepts
        for concept, keywords in KEYWORD_FALLBACKS.items():
            if concept in cleaned_path:
                # Look for a VALID URL that matches any of these keywords
                for v in valid_urls:
                    v_lower = v.lower()
                    if any(kw in v_lower for kw in keywords):
                        # Found a match! e.g. "tithes-and-offerings" contains "tithe"
                        return f"[{label}]({v})"
                        
        # Fallback: STRICT GUARANTEE
        # If we reached here, the URL is NOT in our database and didn't match any heuristic.
        # We MUST replace it to prevent 404s/Hallucinations.
        return f"[{label}]({fallback_url})"
    # Regex for [text](url)
    pattern = r'\[([^\]]+)\]\((https?://[^)]+|/[^)]+)\)'
    return re.sub(pattern, replace_link, text)

# Initialize FastAPI App
app = FastAPI(title="Church Assistant API")

# **CRITICAL:** Add CORS middleware to allow the widget (from another domain/port) to access this API.
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"], # Allow all origins for local testing. RESTRICT this in production!
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# --- LOAD MODELS (No caching required in API) ---
embedding_model = HuggingFaceEmbeddings(model_name="sentence-transformers/all-mpnet-base-v2")
llm = ChatOpenAI(model_name="gpt-4o-mini", temperature=0.3)

# --- DATA MODELS (For API Input/Output) ---
class ChatRequest(BaseModel):
    message: str
    history: List[dict] = [] # List of {"role": "user/assistant", "content": "..."}
    use_full_context: bool = False # Control the RAG mode from the frontend
    church_id: Optional[str] = None # Support Multi-Tenancy

class ChatResponse(BaseModel):
    response: str
    sources: List[str]

# --- CHAT ENDPOINT ---
@app.post("/chat", response_model=ChatResponse)
async def chat(request: ChatRequest):
    # Determine Context
    church_id = request.church_id
    context_data = DEFAULT_CONTEXT
    
    if church_id and church_id in CHURCH_CONTEXTS:
        context_data = CHURCH_CONTEXTS[church_id]
        print(f"--- Using Context for Church ID: {church_id} ---")
    else:
        # If church_id provided but not found, maybe warn?
        if church_id: print(f"--- Church ID {church_id} not found in loaded contexts, using default ---")
        
    valid_urls = context_data["valid_urls"]
    main_domain = context_data["main_domain"]
    preferred_keyword = context_data.get("preferred_keyword")

    # 1. Database Check
    if not os.path.exists(CHROMA_PATH):
        raise HTTPException(status_code=503, detail="Database not ready. Run ingest.py first.")

    vector_db = Chroma(persist_directory=CHROMA_PATH, embedding_function=embedding_model)
    # OPTIMIZATION: Using MMR (Maximal Marginal Relevance) to fetch diverse results
    # retriever creation moved to retrieval.py
    
    # Convert simple dict history to LangChain Messages
    # OPTIMIZATION: Prune history to last 4 messages to prevent infinite token growth
    recent_history = request.history[-4:] if len(request.history) > 4 else request.history
    
    langchain_history = [
        HumanMessage(content=msg['content']) if msg['role'] == 'user' else AIMessage(content=msg['content'])
        for msg in recent_history
    ]

    # 2. Retrieve Relevant Context (Hybrid Logic)
    final_docs = retrieve_and_rank(request.message, vector_db, valid_urls, preferred_keyword, church_id)
    
    # Construct Context Text
    if request.use_full_context:
        # Full context unsupported in multi-tenant for now/too expensive
        context_text = ""
    else:
        context_text = "\n\n".join([f"[Source: {d.metadata.get('source', 'unknown')}]\n{d.page_content}" for d in final_docs])
        # HARD CAP: Limit context to ~3.7k tokens (15,000 chars) to prevent cost blowouts
        if len(context_text) > 15000:
            context_text = context_text[:15000] + "...(truncated)"

    # DYNAMIC CONTACT URL: Find the best "Contact" page for this specific church
    contact_url = "https://google.com" # Ultimate fallback
    if main_domain: contact_url = main_domain
    
    # Try to find a better one
    for u in valid_urls:
        if "/contact" in u or "/connect" in u:
            contact_url = u
            break

    if not context_text:
        return ChatResponse(response=f"I apologize, that specific detail isn't available on our website right now. However, I can still get you connected! Please visit our [Contact Page]({contact_url}).", sources=[])

    # 3. Generate Answer (Hope Persona)
    prompt_template = ChatPromptTemplate.from_messages([
        ("system", """You are a warm, welcoming, and helpful digital greeter for a church website.
        
    GUIDELINES:
    1. **Persona & Tone (PRIORITY):** You are a friendly church assistant. BE WARM, CHATTY, AND INVITING.
        - **MANDATORY:** Start with a warm, conversational sentence BEFORE listing any information.
        - **NEVER** just output a list. Always speak first.
    2. **Accuracy (NO HALLUCINATIONS):**
        - **CRITICAL:** Use **ONLY** URLs that are explicitly provided in the `Context` below.
        - **NEVER** invent or guess a URL (e.g. do not make up `/care-support` or `/connect`).
        - **VERIFICATION:** If a URL you want to use is NOT listed in the `SOURCE:` fields of the context, **DO NOT USE IT.** This is a hard rule.
        - If you don't have a specific link for a topic, link to the **Home Page** or **Contact Page** found in context.
        - If the answer is NOT in the context, say: "I'm not sure about that specific detail, but I'd love to help you find out!" and then provide a link to the **[Contact Page]({{CONTACT_URL}})**.
    3. **Formatting & Structure (CARDS):**
        - **To make a "Card" in the chat, you MUST use a bullet point WITH A LINK.**
        - **Structure:** `* **[Campus Name](url)**: Service A, Service B...`
        - **Preferred Format:** 
          `* **[Campus Name](url)**`
          `  * Service A`
          `  * Service B`
    4. **Links & CTA (STRICT DE-DUPLICATION):** 
        - **NEVER** output multiple bullet points that link to the SAME URL. This is critical.
        - **Consolidate:** If "Join Group", "Serve", and "Classes" all link to `/connect`, output **ONE** bullet: 
          `* [Connect Page](url) - Join a group, serve on a team, or take a class.`
        - **Visual Check:** If you see the same blue link twice, you have failed. Merge them.
    5. **Fallbacks & Safety:**
        - **Plan Your Visit:** if no specific `/visit` or `/plan` page exists, link to the **Home Page**.
        - **Home Page Links:** When linking to the Home Page, **ALWAYS** use the base URL (e.g. `https://church.com/`). **NEVER** use a sub-page (like `/team/` or `/about/`) as the Home Page link.
        - **Distress:** Link "Prayer" or "Contact". Avoid specific care groups unless asked.
        - Compassionate tone.
    
    6. **Service Times Logic (CRITICAL):**
        - **GROUP TIMES BY DAY:** Never list the same day multiple times.
            - **BAD:** "Every Sunday at 9:00 AM, Every Sunday at 11:00 AM"
            - **GOOD:** "Sundays at 9:00 AM & 11:00 AM"
        - **NO REPETITION:** Do not use the phrase "Every Sunday" more than once per campus.
        - **ACCURACY:** Check the context carefully. If a campus lists multiple times (e.g. 9:30 & 11:00), you **MUST** list all of them. Do not skip any.
        - **Ignore** one-off event times (e.g., "Special Event") unless specifically asked.
        - If multiple locations exist, list times for **each** location clearly.
            
    7. **Link Naming (ACCURACY):**
        - When creating a link `[Link Text](url)`, the "Link Text" MUST match the actual page title or header found in the context for that URL.
        - **DO NOT INVENT NAMES.** Do not call a page "Prayer Page" if the title is "Care & Support".
            
    Context: {context}
    """),
        *langchain_history, # Unpack the chat history
        ("human", "{question}")
    ])
    
    chain = prompt_template | llm
    # Removed special_links argument as requested
    response = chain.invoke({"context": context_text, "question": request.message})
    
    # 3.25 Inject Dynamic Contact URL
    # Replace the placeholder with the actual contact URL found above (Robust Regex)
    import re
    # Match {{CONTACT_URL}}, {{ CONTACT_URL }}, or even %7BCONTACT_URL%7D (url encoded)
    pattern = r'(?:\{+\s*CONTACT_URL\s*\}+|%7B\s*CONTACT_URL\s*%7D)'
    response.content = re.sub(pattern, contact_url, response.content, flags=re.IGNORECASE)
    
    # 3.5 Validate Links (Deterministic Guarantee)
    validated_content = validate_and_fix_links(response.content, valid_urls, contact_url, main_domain)
    
    # 4. Extract Sources
    unique_sources = list(set([d.metadata.get('source', '') for d in final_docs]))
    
    return ChatResponse(response=validated_content, sources=unique_sources)

from fastapi.responses import HTMLResponse, FileResponse

@app.get("/hope-chatbot.js", response_class=FileResponse)
async def get_widget_js():
    return os.path.join(BASE_DIR, "hope-chatbot.js")

@app.get("/", response_class=HTMLResponse)
async def get_widget():
    # Attempt to locate the widget file
    widget_path = os.path.join(BASE_DIR, "hope_widget.html")
    with open(widget_path, "r", encoding="utf-8") as f:
        return f.read()

if __name__ == "__main__":
    import uvicorn
    # 0.0.0.0 allows access from other devices on the network
    uvicorn.run(app, host="0.0.0.0", port=8004)