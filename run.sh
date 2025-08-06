#!/bin/bash

BACKEND_PID=""
CLEANUP_DONE=false

cleanup() {
    if [[ "$CLEANUP_DONE" == "true" ]]; then
        return  
    fi
    
    CLEANUP_DONE=true
    echo "Stopping services..."

    if [[ -n "$BACKEND_PID" ]]; then
        echo "Stopping backend (PID: $BACKEND_PID)..."
        kill $BACKEND_PID 2>/dev/null
    fi

    sleep 2
    
    # Force kill
    if [[ -n "$BACKEND_PID" ]] && kill -0 $BACKEND_PID 2>/dev/null; then
        echo "Force killing backend..."
        kill -9 $BACKEND_PID 2>/dev/null
    fi
    
    echo "All services stopped."
}



if [[ ! -d backend/.venv || ! -f downward/builds/release/bin/downward ]]; then
    echo "You need to run 'build.sh' first to set up the backend environment."
    echo "Do you want to run 'build.sh' now? (y/n)"
    read -r answer
    if [[ "$answer" == "y" || "$answer" == "Y" ]]; then
        ./build.sh
    else
        echo "Exiting without starting services."
    exit 1
    fi
fi

trap cleanup INT TERM EXIT

echo "Starting backend..."
cd backend
./run.sh &
BACKEND_PID=$!

sleep 5

echo "Starting frontend..."
cd ../frontend  
npm run dev
