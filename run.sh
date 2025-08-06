#!/bin/bash

BACKEND_PID=""
FRONTEND_PID=""

cleanup() {
    echo "Stopping services..."

    if [[ -n "$BACKEND_PID" ]]; then
        echo "Stopping backend (PID: $BACKEND_PID)..."
        kill $BACKEND_PID 2>/dev/null
    fi
    
    if [[ -n "$FRONTEND_PID" ]]; then
        echo "Stopping frontend (PID: $FRONTEND_PID)..."
        kill $FRONTEND_PID 2>/dev/null
    fi
    sleep 2
    
    # Force kill
    if [[ -n "$BACKEND_PID" ]] && kill -0 $BACKEND_PID 2>/dev/null; then
        echo "Force killing backend..."
        kill -9 $BACKEND_PID 2>/dev/null
    fi
    
    if [[ -n "$FRONTEND_PID" ]] && kill -0 $FRONTEND_PID 2>/dev/null; then
        echo "Force killing frontend..."
        kill -9 $FRONTEND_PID 2>/dev/null
    fi
    
    echo "All services stopped."
}

trap cleanup INT TERM EXIT

echo "Starting backend..."
cd backend
./run.sh &
BACKEND_PID=$!

sleep 5

echo "Starting frontend..."
cd ../frontend  
npm run dev
FRONTEND_PID=$!

wait