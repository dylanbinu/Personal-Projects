import os
import sys
import json

from langchain_chroma import Chroma
from langchain_huggingface import HuggingFaceEmbeddings
from langchain_text_splitters import RecursiveCharacterTextSplitter
from langchain_core.documents import Document

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.join(BASE_DIR, "..")
CHROMA_PATH = os.path.join(PROJECT_ROOT, "chroma_db")
DATA_FILE = os.path.join(PROJECT_ROOT, "scraped_data.jsonl")

def main():
    import argparse
    import shutil
    
    parser = argparse.ArgumentParser(description="Ingest Church Data")
    parser.add_argument("--church_id", type=str, help="Unique ID for the church (e.g., 'my_church')")
    parser.add_argument("--input_file", type=str, default="scraped_data.jsonl", help="Input JSONL file")
    parser.add_argument("--reset", action="store_true", help="Wipe the entire database before ingesting")
    
    args = parser.parse_args()

    print("="*50)
    print("   INGESTING KNOWLEDGE BASE")
    
    # Handle DB Reset
    if args.reset:
        if os.path.exists(CHROMA_PATH):
            try:
                shutil.rmtree(CHROMA_PATH)
                print(f"   [RESET] Cleared existing database at {CHROMA_PATH}")
            except PermissionError:
                print(f"[ERROR] Could not clear DB. Server might be running.", file=sys.stderr)
                sys.exit(1)
            except Exception as e:
                print(f"[ERROR] {e}", file=sys.stderr)
                sys.exit(1)
        os.makedirs(CHROMA_PATH, exist_ok=True)
    else:
        print(f"   [INFO] Appending to existing database at {CHROMA_PATH}")
        # PRE-CLEANUP: If we are appending for a specific church, we should delete OLD data for that church first
        if args.church_id:
            try:
                print(f"   [INFO] Removing old data for church_id: {args.church_id}")
                # We need to instantiate Chroma to delete
                temp_db = Chroma(persist_directory=CHROMA_PATH, embedding_function=HuggingFaceEmbeddings(model_name="sentence-transformers/all-mpnet-base-v2"))
                
                # Get all IDs for this church
                # This is expensive but necessary if we don't have a direct metadata delete API in this version of valid langchain wrapper
                # Optimization: In real production, use a dedicated Vector DB server (Qdrant/Weaviate) that supports delete_by_filter
                
                # Retrieve all docs to find IDs (Slow for massive DBs, acceptable for local/small scale)
                # Actually, Chroma has a `get` method
                existing_docs = temp_db.get(where={"church_id": args.church_id})
                if existing_docs and existing_docs['ids']:
                    ids_to_delete = existing_docs['ids']
                    print(f"   [INFO] Deleting {len(ids_to_delete)} existing chunks for this church...")
                    temp_db.delete(ids=ids_to_delete)
                else:
                    print("   [INFO] No existing data found for this church.")
            except Exception as e:
                print(f"   [WARN] Could not clean up old data: {e}")

    # Determine Input File
    if os.path.isabs(args.input_file):
        data_path = args.input_file
    else:
        data_path = os.path.join(PROJECT_ROOT, args.input_file)

    if not os.path.exists(data_path):
        print(f"[ERROR] Data file not found: {data_path}", file=sys.stderr)
        sys.exit(1)

    # 1. Load Documents
    documents = []
    with open(data_path, "r", encoding="utf-8") as f:
        for line in f:
            if line.strip():
                try:
                    data = json.loads(line)
                    metadata = {"source": data["source"]}
                    if args.church_id:
                        metadata["church_id"] = args.church_id
                        
                    doc = Document(
                        page_content=data["content"],
                        metadata=metadata
                    )
                    documents.append(doc)
                except:
                    continue
    
    print(f"   Loaded {len(documents)} distinct pages from {args.input_file}.")

    # 2. Split Text
    text_splitter = RecursiveCharacterTextSplitter(
        chunk_size=2000,
        chunk_overlap=300,
        separators=["\n\n", "\n", "###", "##", " ", ""]
    )
    chunks = text_splitter.split_documents(documents)
    print(f"   Created {len(chunks)} searchable chunks.")

    # 3. Vectorize
    embedding_model = HuggingFaceEmbeddings(model_name="sentence-transformers/all-mpnet-base-v2")
    
    # If Church ID is provided and NOT resetting, strictly we should delete old data for this church first
    # to avoid duplication. 
    # NOTE: Chroma basic client doesn't make delete-by-metadata easy without loading it first.
    # For now, we assume the user manages this (or uses --reset for full rebuild).
    # Ideally: vector_store.delete(filter={"church_id": args.church_id}) if possible.
    
    # Save to Disk
    BATCH_SIZE = 50
    for i in range(0, len(chunks), BATCH_SIZE):
        batch = chunks[i:i+BATCH_SIZE]
        Chroma.from_documents(
            documents=batch,
            embedding=embedding_model,
            persist_directory=CHROMA_PATH
        )
        print(f"   Processed batch {i//BATCH_SIZE + 1}...")

    print("-" * 50)
    print("Ingestion Complete.")

if __name__ == "__main__":
    main()