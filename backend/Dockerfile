# Multi-stage build for Fast Downward compilation
FROM python:3.11-slim AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake \
    && rm -rf /var/lib/apt/lists/*

# Compile Fast Downward
WORKDIR /app/downward
COPY downward/ .
RUN python3 build.py

# Production image
FROM python:3.11-slim AS runtime

WORKDIR /app/backend

# Install Python dependencies
COPY backend/requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy backend code
COPY backend/ .

# Copy Fast Downward source + compiled binaries
COPY downward/ /app/downward/
COPY --from=builder /app/downward/builds/release /app/downward/builds/release

# Create working directories
RUN mkdir -p temp_files out_graphs

CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]