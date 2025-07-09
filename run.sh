#!/bin/bash
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"

.venv/bin/python -m uvicorn --app-dir backend main:app #--reload
