# 🤖 Universal Web RAG Chatbot (Smart Search Edition)

A powerful, local AI tool that allows you to "chat" with any website.

This tool scrapes a target website using **Async Playwright** (with stealth mode), converts the content into structured **Markdown**, ingests it into a local **Hybrid Database** (Vector + Keyword), and uses OpenAI to answer questions based strictly on that data via a polished Web Interface.

## 🚀 Features
* **Smart Hybrid Search:** Combines **Vector Search** (ChromaDB) for conceptual understanding with **BM25 Keyword Search** for exact matches (names, dates, IDs).
* **Universal Async Scraper:** Uses an asynchronous headless browser to handle dynamic JavaScript-heavy sites (React, Angular) faster than standard scrapers.
* **Markdown Storage:** Saves data as `.md` files to preserve website structure (headers, lists, links) for better AI comprehension.
* **Web Interface:** A professional UI built with **Streamlit** features a sidebar for data management and a chat window with persistent memory.
*   **Stealth Mode:** Automatically bypasses basic bot protections by mimicking human browser behavior.
*   **Link Accuracy Guard:** Prevents the bot from inventing (hallucinating) false URLs.
*   **Dynamic Visit Planning:** Automatically finds the correct "Plan Your Visit" or "Home" page for any church site without manual configuration.
*   **Zero-Hallucination Mode:** Strictly enforces that all links provided in the chat actually exist in the website context.

---

## 🛠️ Prerequisites

1.  **Python 3.10+**: Ensure Python is installed and added to your system PATH.
2.  **OpenAI API Key**: You need a valid API key.
    *   Create a `.env` file in the project root.
    *   Add `OPENAI_API_KEY=sk-proj-...` to it.

---

## ⚡ Quick Start (One-Click Setup)

We have provided a batch script to handle installation, updates, and factory resets automatically.

1.  Navigate to the `scripts/` folder.
2.  Double-click **`setup.bat`**.

**This script will:**
1.  Check your Python installation.
2.  Create (or repair) the virtual environment (`venv`).
3.  Install all dependencies (AI libraries, Browsers, Search Engines).
4.  **Automatically launch the Web Interface.**

---

## 📖 How to Use

### 1. Launch the App
For daily use, you don't need to run setup again.
* Double-click **`scripts/launch_app.bat`**.
* This opens the chatbot in your default web browser.

### 2. Load a Website
The app starts empty. To teach it a new website:
1.  Open the **Sidebar** (left side of the app).
2.  Paste a URL (e.g., `https://www.spacex.com/vehicles/starship/`).
3.  Click **"Load Website Data"**.
4.  Watch the status log as it scrapes, cleans, and indexes the data.

### 3. Chat
Once the green "✅ Done!" message appears, simply type your question in the main chat box.
* *Example:* "What is the payload capacity?"
* *Example:* "How many engines does it have?"

---

## 📂 Project Structure

```text
Project Root/
├── code/                   # PYTHON SOURCE CODE
│   ├── app.py              # Main Application (UI & Brain)
│   ├── webscrape.py        # Async Scraper (Stealth Mode)
│   ├── ingest.py           # Hybrid Loader (Markdown -> Chroma/BM25)
│   └── reset.py            # Database Cleaner
│
├── scripts/                # BATCH FILES (Run these)
│   ├── setup.bat           # Installer & Factory Reset Tool
│   └── launch_app.bat      # Daily Launcher (Starts Web UI)
│
├── venv/                   # Virtual Environment (Libraries)
├── chroma_db/              # Local Vector Database (Auto-generated)
├── scraped_data.md         # The raw structured text from the website
└── requirements.txt        # List of dependencies