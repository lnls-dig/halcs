#!/usr/bin/env bash

TOP_DIR=..
BOARD_TYPE="afcv3"
REMOTE_USERNAME=$1
REMOTE_IP=$2
REMOTE_DIR=$3

function usage {
    echo "Usage: $0 <remote_username> <remote_ip> <remote_dir>"
}

if [ -z "$REMOTE_USERNAME" ]; then
    echo "\"REMOTE_USERNAME\" variable unset."
    usage
    exit 1
fi

if [ -z "$REMOTE_IP" ]; then
    echo "\"REMOTE_IP\" variable unset."
    usage
    exit 1
fi

if [ -z "$REMOTE_DIR" ]; then
    echo "\"REMOTE_DIR\" variable unset."
    usage
    exit 1
fi

CREATE_DIR_CMD="mkdir -p ${REMOTE_DIR}"

# Change working directory
cd ${TOP_DIR}

# Create remote dirs
echo "Creating remote directories on ${REMOTE_USERNAME}@${REMOTE_IP}"
ssh -l ${REMOTE_USERNAME} ${REMOTE_IP} "${CREATE_DIR_CMD}"

# Find all source files
SRC_FILES=$(find . -type f ! -iname "*.[o|a]" ! -iname "*orig" \
    ! -iname "*.swp" ! -iname "*.swap" ! -iname "*.txt" ! -iname "*.pyc" \
    ! -path "./.git*")

echo "Copying files to ${REMOTE_USERNAME}@${REMOTE_IP}:${REMOTE_DIR}"
rsync -Rpr ${SRC_FILES} ${REMOTE_USERNAME}@${REMOTE_IP}:${REMOTE_DIR}
