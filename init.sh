#!/bin/bash

REQUIRED_ARGS=2
BIN_DIR=/usr/local/bin

if [ "$(id -u)" != "0" ]
then
	echo "This must be run as root"
	exit 1
fi

if [ $# -lt $REQUIRED_ARGS ]
then
	echo "Error: Wrong arguments!"
	echo "Usage: `basename $0` {transport type = [ipc|tcp]} {broker endpoint} [log directory]"
	exit 1
fi

transport=$1
broker_endp=$2
log_dir=$3
log_dir_opt=

if [ -z "$log_dir" ]
then
    log_dir_opt=
else
    log_dir_opt="-l $log_dir"
fi

# Launch Device Manager
${BIN_DIR}/dev_mngr -v -b ${transport}://${broker_endp} ${log_dir_opt} &

# Wait until initial workers have been created
sleep 1
# Change IPC permissions if transport = ipc
if [ "$transport" = "ipc" ]
then
    chmod 777 $broker_endp
fi
