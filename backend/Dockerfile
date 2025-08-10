FROM python:3.11-slim AS builder
RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app/downward
COPY downward/ .
RUN python3 build.py

FROM python:3.11-slim AS runtime
WORKDIR /app/backend
COPY backend/requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt
COPY backend/ .
COPY downward/ /app/downward/
COPY --from=builder /app/downward/builds/release /app/downward/builds/release
RUN mkdir -p temp_files out_graphs
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]