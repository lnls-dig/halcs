#!/bin/bash

TOP_DIR=..
REMOTE_HOME="/home"
REMOTE_USERNAME="lnls-bpm"
REMOTE_IP=10.0.18.41
LOCAL_IP=10.0.18.35
REMOTE_DIR="bpm-sw-deploy"
BOARD_TYPE="afcv3"

REMOTE_DEPLOY_DIR="${REMOTE_HOME}/${REMOTE_USERNAME}/${REMOTE_DIR}"
REMOTE_LOG_DIR="/media/remote_logs"

CREATE_DIR_CMD="mkdir -p ${REMOTE_DEPLOY_DIR}"
MOUNT_REMOTE_LOG_CMD="sshfs ${whoami}@${LOCAL_IP}:~/remote_logs ${REMOTE_LOG_DIR}"
COMPILE_CMD="make mrproper && make DBE_DBG=y FMC130M_4CH_TYPE=passive \
    BOARD=${BOARD_TYPE} && make install"
RUN_CMD="./init.sh tcp ${REMOTE_IP} ${REMOTE_LOG_DIR}"

# Change working directory
cd ${TOP_DIR}

# Create remote dirs
echo "Creating remote directories on ${REMOTE_USERNAME}@${REMOTE_IP}"
ssh -l ${REMOTE_USERNAME} ${REMOTE_IP} "${CREATE_DIR_CMD}"

# Mount remote log directory
echo "Mounting remote LOG directory to ${REMOTE_LOG_DIR}"
ssh -l ${REMOTE_USERNAME} ${REMOTE_IP} "${MOUNT_REMOTE_LOG_CMD}"

# Find all source files
SRC_FILES=$(find . -type f -type l ! -iname "*.[o|a]" ! -iname "*orig" \
    ! -iname "*.swp" ! -iname "*.swap" ! -iname "*.txt" \
    ! -path "./majordomo*" ! -path "./.git*")
SYM_FILES=$(find . -type l ! -iname "*.[o|a]" ! -iname "*orig" \
    ! -iname "*.swp" ! -iname "*.swap" ! -iname "*.txt" \
    ! -path "./majordomo*" ! -path "./.git*")

echo "Copying files to ${REMOTE_USERNAME}@${REMOTE_IP}:${REMOTE_DEPLOY_DIR}"
rsync -Rpr ${SRC_FILES} ${REMOTE_USERNAME}@${REMOTE_IP}:${REMOTE_DEPLOY_DIR}
rsync -Rpr ${SYM_FILES} ${REMOTE_USERNAME}@${REMOTE_IP}:${REMOTE_DEPLOY_DIR}

echo "Compiling source files with \"${COMPILE_CMD}\""
ssh -l ${REMOTE_USERNAME} ${REMOTE_IP} "cd ${REMOTE_DIR} && ${COMPILE_CMD} && ${RUN_CMD}"

