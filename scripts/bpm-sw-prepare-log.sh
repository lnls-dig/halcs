#!/bin/bash

LOG_DIR="/media/remote_logs"
REMOTE_HOST="10.0.18.35"
REMOTE_LOG_DIR="/export/remote_logs"

#rm -rf ${LOG_DIR}
#mkdir -p ${LOG_DIR}
#mount -t nfs -o proto=tcp,port=2049 ${REMOTE_HOST}:${REMOTE_LOG_DIR} ${LOG_DIR}
#sshfs -o allow_other -o kernel_cache -o auto_cache -o reconnect -o compression=no -o cache_timeout=600 -o ServerAliveInterval=15 ${REMOTE_HOST}:${REMOTE_LOG_DIR} ${LOG_DIR}

