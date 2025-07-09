
import os
from subprocess import Popen

import json

from fastapi import FastAPI, HTTPException, UploadFile, status, File
from fastapi.encoders import jsonable_encoder
from fastapi.responses import JSONResponse
from pydantic import BaseModel

# ================================
TEMP_FILE_DIR = "temp_pddl_files"
assert os.path.exists(TEMP_FILE_DIR)

OUT_DIR = "out_graphs"
assert os.path.exists(OUT_DIR)

FPATH1 = os.path.join(TEMP_FILE_DIR, "f1.pddl")
FPATH2 = os.path.join(TEMP_FILE_DIR, "f2.pddl")
LOG_PATH = os.path.join(TEMP_FILE_DIR, "log.txt")

ALGNAME = "graph_only"

DOWNWARD_TIMEOUT = 300 # s

# ================================
def temp_files() -> list[str]:
    return os.listdir(TEMP_FILE_DIR)

def has_pddl_files() -> bool:
    return os.path.exists(FPATH1) and os.path.exists(FPATH2)

def cg_path() -> str:
    return os.path.join(OUT_DIR, "causal_graph.json")

def dtg_path(ind: int) -> str:
    return os.path.join(OUT_DIR, f"dtg_{ind}.json")

# ================================
app = FastAPI()

async def run_downward() -> bool:
    cmd = f"./fast-downward.py {FPATH1} {FPATH2} --search \"{ALGNAME}(\\\"{OUT_DIR}\\\")\""
    p = Popen(['watch', 'ls'])  # something long running
    # ... do other stuff while subprocess is running
    retcode = p.wait(DOWNWARD_TIMEOUT)
    fine = retcode == 0

    res = p.communicate() # TODO: Check if this works after wait()

    if not fine:
        with open(LOG_PATH, "wb") as logfile:
            logfile.write(res)

    return fine

@app.post("/upload")
async def upload_and_execute(file1: UploadFile, file2: UploadFile):
    modifies = has_pddl_files()
    with open(FPATH1, "w", encoding="utf-8") as doc:
        doc.write(file1.read())
    with open(FPATH2, "w", encoding="utf-8") as doc:
        doc.write(file2.read())
    
    action = "Modified" if modifies else "Uploaded"

    success = await run_downward()

    if not success:
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_ENTITY,
            detail="Fastdownward ERROR!"
        )

    return f"{action} PDDL files and ran Fastdownward"

@app.get("/cg")
def causal_graph():
    path = cg_path()

    if not os.path.exists(path):
        raise HTTPException(
            status_code=status.HTTP_405_METHOD_NOT_ALLOWED,
            detail=f'No Causal Graph created yet!',
        )    
    
    with open(path, "r", encoding="utf-8") as file:
        return JSONResponse(content=file.read())


@app.get("/dtg/{id}")
def domain_t_graph(id: int):
    path = dtg_path(id)

    if not os.path.exists(path):
        raise HTTPException(
            status_code=status.HTTP_405_METHOD_NOT_ALLOWED,
            detail=f'No Domain Transition Graph with ID {id} created yet!',
        )

    with open(path, "r", encoding="utf-8") as file:
        return JSONResponse(content=file.read())
