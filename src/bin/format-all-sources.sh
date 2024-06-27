#!/bin/bash

SCRIPT_DIR="$(realpath "$( dirname "${BASH_SOURCE[0]}" )" )"
SCRIPT_NAME="$(basename "${BASH_SOURCE[0]}")"
SCRIPT_STEM="${SCRIPT_NAME%.*}"

# call the python script on the same folder with same name just with .py extension
python3 ${SCRIPT_DIR}/${SCRIPT_STEM}.py "$@"