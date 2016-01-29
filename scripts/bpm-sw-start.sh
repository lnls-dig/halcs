#!/bin/sh

BIN_DIR=$1

function usage {
    echo "Usage: $0 <bin directory> [options]"
}

if [ -z "$BIN_DIR" ]; then
    echo "\"BIN_DIR\" variable unset."
    usage
    exit 1
fi

for opt in "${@:2}"
do
    OPTS+=("${item}")
done

${BIN_DIR}/dev_io ${EXTRA_FLAGS[@]}
