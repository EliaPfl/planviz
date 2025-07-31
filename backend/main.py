
import asyncio
import os
from datetime import datetime

from fastapi import FastAPI, HTTPException, UploadFile, status
from fastapi.responses import PlainTextResponse

# ================================
TEMP_FILE_DIR = "temp_files"
assert os.path.exists(TEMP_FILE_DIR)

OUT_DIR = "out_graphs"
assert os.path.exists(OUT_DIR)

FPATH1 = os.path.join(TEMP_FILE_DIR, "f1.pddl")
FPATH2 = os.path.join(TEMP_FILE_DIR, "f2.pddl")
LOG_PATH = os.path.join(TEMP_FILE_DIR, "log.txt")

ALGNAME = "graph_only"

DOWNWARD_TIMEOUT = 300  # s

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
    cmd = f"./downward/fast-downward.py {FPATH1} {FPATH2} --search \"{ALGNAME}(\\\"{OUT_DIR}\\\")\""
    proc = await asyncio.create_subprocess_shell(
        cmd,
        stdout=asyncio.subprocess.PIPE,
        stderr=asyncio.subprocess.PIPE)

    out, err = await proc.communicate()

    fine = proc.returncode == 0
    if not fine:
        print(f"ERROR executing {cmd}")
        with open(LOG_PATH, "wb") as logfile:
            logfile.write(f"ERROR {datetime.now()}\n".encode())
            logfile.write(b"[stderr]:\n")
            logfile.write(err)
            logfile.write(b"\n[stdout]:\n")
            logfile.write(out)

    return fine

async def identify_pddl_files(file1: UploadFile, file2: UploadFile):
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

@app.post("/upload")
async def upload_and_execute(files: list[UploadFile]):
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
    path = dtg_path(id)

    if not os.path.exists(path):
        raise HTTPException(
            status_code=status.HTTP_405_METHOD_NOT_ALLOWED,
            detail=f'No Domain Transition Graph with ID {id} created yet!',
        )

    with open(path, "r", encoding="utf-8") as file:
        return file.read()