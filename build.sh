#!/bin/bash
echo "Building backend..."
cd backend
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
mkdir out_graphs
mkdir temp_files

echo "Building frontend..."
cd ../frontend
npm install

echo "Building downward..."
cd ../downward
./build.py