#!/bin/bash
set -e

# Usage: ./myscript.sh <batchname>.txt | <batchname>.failed [jobs]
# Example: ./myscript.sh KO_25_07.txt 4   # run with 4 parallel jobs
# If jobs is not given, default is sequential execution

if [[ $# -lt 1 ]]; then
    echo "Usage: $0 <batchname>.txt | <batchname>.failed [jobs]"
    exit 1
fi

RUNLIST_FILE=$1
JOBS=$2   # optional argument for parallel jobs

# Extract batch name: remove everything after the first dot
BATCH_NAME=$(basename "$RUNLIST_FILE" | sed -E 's/\..*$//')

INPUT_DATA_PATH=/mnt/d/KoBRA_exp/KO_25_07
OUTPUT_BASE=./OUTPUT

OUTPUT_DIR="${OUTPUT_BASE}/${BATCH_NAME}"
mkdir -p "${OUTPUT_DIR}"

# Failed run list file (reset at start)
FAIL_LOG="${OUTPUT_DIR}/${BATCH_NAME}.failed"
: > "$FAIL_LOG"

process_run() {
    RUNNUMBER=$1
    INPUT_FILE="${INPUT_DATA_PATH}/Run${RUNNUMBER}/FADC0.dat"
    HIST_FILE="${OUTPUT_DIR}/Hists_${RUNNUMBER}.root"
    TREE_FILE="${OUTPUT_DIR}/Tree_${RUNNUMBER}.root"
    LOG_FILE="${OUTPUT_DIR}/Run${RUNNUMBER}.log"

    echo "Processing ${BATCH_NAME} Run ${RUNNUMBER}..."
    ./NKFADC_Unpacker -tw 150 -i "$INPUT_FILE" -hp 8181 \
        -ho "$HIST_FILE" -to "$TREE_FILE" \
        2>&1 | tee "$LOG_FILE"

    # If unpacker failed, record run number in failed list
    if [[ ${PIPESTATUS[0]} -ne 0 ]]; then
        echo "$RUNNUMBER" >> "$FAIL_LOG"
    fi
}

export -f process_run
export INPUT_DATA_PATH OUTPUT_DIR BATCH_NAME FAIL_LOG

if [[ -z "$JOBS" ]]; then
    # Sequential execution
    while read RUNNUMBER; do
        [[ -z "$RUNNUMBER" || "$RUNNUMBER" =~ ^# ]] && continue
        process_run "$RUNNUMBER"
    done < "$RUNLIST_FILE"
else
    # Parallel execution with specified number of jobs
    grep -v '^#' "$RUNLIST_FILE" | parallel -j "$JOBS" process_run {}
fi
