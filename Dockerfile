
FROM ubuntu:24.04 AS fd_base

EXPOSE 8000

WORKDIR /app

# installing necessary packages
RUN apt-get update -y
RUN apt-get install cmake -y
RUN apt-get install -y python3-full python3-pip python3-venv

# Copying the files from repo
# COPY requirements.txt .
COPY . .

# Adding new search algorithm to FastDownward
ADD custom_search fast-downward/src/search/search_algorithms

# Adding a venv to run Python
RUN mkdir .venv
RUN python3 -m venv .venv
# RUN . venv/bin/activate && your command here

# Running python commands to install necessary dependencies & build Fastdownward 
RUN .venv/bin/pip install -r requirements.txt
RUN .venv/bin/python fast-downward/build.py VERBOSE=true

# Modifying privileges of run.sh
RUN chmod +x run.sh

# == SECOND STEP ===================
FROM fd_base

# directory to store the problem and domain file to 
# (to later use it in fastdownward command)

CMD ["run.sh"]