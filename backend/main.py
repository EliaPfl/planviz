import asyncio
import os
from datetime import datetime
import sys

from fastapi import FastAPI, HTTPException, UploadFile, status
from fastapi.responses import PlainTextResponse

# File system configuration
TEMP_FILE_DIR = "temp_files"
assert os.path.exists(TEMP_FILE_DIR)

OUT_DIR = "out_graphs"
assert os.path.exists(OUT_DIR)

FPATH1 = os.path.join(TEMP_FILE_DIR, "f1.pddl")  # Domain file
FPATH2 = os.path.join(TEMP_FILE_DIR, "f2.pddl")  # Problem file
LOG_PATH = os.path.join(TEMP_FILE_DIR, "log.txt")

ALGNAME = "graph_only"  # Custom Fast Downward algorithm

DOWNWARD_TIMEOUT = 300  # s

# Utility functions
def temp_files() -> list[str]:
    return os.listdir(TEMP_FILE_DIR)

def has_pddl_files() -> bool:
    return os.path.exists(FPATH1) and os.path.exists(FPATH2)

def cg_path() -> str:
    return os.path.join(OUT_DIR, "causal_graph.json")

def landmark_path() -> str:
    return os.path.join(OUT_DIR, "landmark_graph.json")

def dtg_path(ind: int) -> str:
    return os.path.join(OUT_DIR, f"dtg_{ind}.json")

# FastAPI app
app = FastAPI()

async def run_downward() -> bool:
    """Execute Fast Downward planner with graph-only search"""
    backend_dir = os.path.abspath(os.path.dirname(__file__))
    script = os.path.abspath(os.path.join(backend_dir, "..", "downward", "fast-downward.py"))

    python_bin = sys.executable or "python3"

    args = [
        python_bin, script,
        FPATH1, FPATH2,
        "--search", f'{ALGNAME}("{OUT_DIR}")'
    ]

    proc = await asyncio.create_subprocess_exec(
        *args,
        stdout=asyncio.subprocess.PIPE,
        stderr=asyncio.subprocess.PIPE,
        cwd=backend_dir, 
    )
    out, err = await proc.communicate()


    fine = proc.returncode == 0
    if not fine:
        print(f"ERROR executing: {' '.join(args)}")
        # Log error details
        with open(LOG_PATH, "wb") as logfile:
            logfile.write(f"ERROR {datetime.now()}\n".encode())
            logfile.write(b"[stderr]:\n")
            logfile.write(err)
            logfile.write(b"\n[stdout]:\n")
            logfile.write(out)

    return fine

async def identify_pddl_files(file1: UploadFile, file2: UploadFile):
    """Identify which file is domain vs problem"""
    content1 = await file1.read()
    content2 = await file2.read()
    
    await file1.seek(0)
    await file2.seek(0)
    
    text1 = content1.decode('utf-8').lower()
    text2 = content2.decode('utf-8').lower()
    
    if '(define (domain' in text1:
        return file1, file2
    elif '(define (domain' in text2:
        return file2, file1 
    else:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="Could not identify domain and problem files"
        )

# API Endpoints
@app.post("/upload")
async def upload_and_execute(files: list[UploadFile]):
    """Upload PDDL files and run Fast Downward graph generation"""
    if len(files) != 2:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="Please upload two PDDL files."
        )
    
    modifies = has_pddl_files()
    domain, problem = await identify_pddl_files(files[0], files[1])
    with open(FPATH1, "wb") as doc:
        doc.write(await domain.read())
    with open(FPATH2, "wb") as doc:
        doc.write(await problem.read())

    action = "Modified" if modifies else "Uploaded"

    success = await run_downward()

    if not success:
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_ENTITY,
            detail="Fastdownward ERROR!"
        )

    return f"{action} PDDL files and ran Fastdownward\n"

@app.get("/cg", response_class=PlainTextResponse)
def causal_graph():
    """Return causal graph JSON"""
    path = cg_path()

    if not os.path.exists(path):
        raise HTTPException(
            status_code=status.HTTP_405_METHOD_NOT_ALLOWED,
            detail=f'No Causal Graph created yet!',
        )

    with open(path, "r", encoding="utf-8") as file:
        return file.read()

@app.get("/dtg/{id}", response_class=PlainTextResponse)
def domain_t_graph(id: int):
    """Return domain transition graph JSON for variable ID"""
    path = dtg_path(id)

    if not os.path.exists(path):
        raise HTTPException(
            status_code=status.HTTP_405_METHOD_NOT_ALLOWED,
            detail=f'No Domain Transition Graph with ID {id} created yet!',
        )

    with open(path, "r", encoding="utf-8") as file:
        return file.read()

@app.get("/landmark", response_class=PlainTextResponse)
def landmark_graph():
    """Return landmark graph JSON"""
    path = landmark_path()

    if not os.path.exists(path):
        raise HTTPException(
            status_code=status.HTTP_405_METHOD_NOT_ALLOWED,
            detail='No Landmark Graph created yet!',
        )

    with open(path, "r", encoding="utf-8") as file:
        return file.read()