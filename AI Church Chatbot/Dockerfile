# Use official Python runtime as a parent image
FROM python:3.10-slim

# Set the working directory to the project root in the container
WORKDIR /app

# Copy the requirements from root
COPY requirements.txt .

# Install dependencies
RUN pip install --no-cache-dir -r requirements.txt

# Copy the entire project path (code + chroma_db + scraped_data) -> /app
COPY . .

# Change to code directory to run the server
WORKDIR /app/code

# Expose port
EXPOSE 7860

# Run server
CMD ["uvicorn", "server:app", "--host", "0.0.0.0", "--port", "7860"]
