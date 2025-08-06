#!/bin/bash

python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt

mkdir out_graphs
mkdir temp_files