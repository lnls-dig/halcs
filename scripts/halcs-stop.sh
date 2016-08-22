#!/bin/sh

BIN_INSTANCE=$1

function usage {
    echo "Usage: $0 <bin instance>"
}

if [ -z "$BIN_INSTANCE" ]; then
    echo "\"BIN_INSTANCE\" variable unset."
    usage
    exit 1
fi

# FIXME: Find correct dev_io instance looking at all running processes
TARGET_PID=$(ps aux | grep -Ei "dev_io.*/dev/fpga-${BIN_INSTANCE}.*" | head -1 | awk '{print $2}')

kill ${TARGET_PID}
