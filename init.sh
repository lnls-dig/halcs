#!/bin/bash

REQUIRED_ARGS=0
BIN_DIR=/usr/local/bin

if [ "$(id -u)" != "0" ]
then
	echo "This must be run as root"
	exit 1
fi

if [ $# -lt $REQUIRED_ARGS ]
then
	echo "Error: Wrong arguments!"
	echo "Usage: `basename $0`"
	exit 1
fi

# Launch Device Manager
${BIN_DIR}/dev_mngr &

# Wait until initial workers have been created
sleep 1
# Change IPC permissions if transport = ipc
if [ "$transport" = "ipc" ]
then
    chmod 777 $broker_endp
fi
