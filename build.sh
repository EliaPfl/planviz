#!/bin/bash
echo "Building backend..."
cd backend
./build.sh
source .venv/bin/activate

echo "Building frontend..."
cd ../frontend
npm install

echo "Building downward..."
cd ../downward
./build.py