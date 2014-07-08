#!/bin/bash

# Broker Endpoint
EXPECTED_ARGS=2

if [ "$(id -u)" != "0" ]
then
	echo "This must be run as root"
	exit 1
fi

if [ $# -ne $EXPECTED_ARGS ]
then
	echo "Error: Wrong arguments!"
	echo "Usage: `basename $0` {transport type = [ipc|tcp]} {broker endpoint}"
	exit 1
fi

transport=$1
broker_endp=$2
log_filename="dev_mngr.log"

# Launch Device Manager
./dev_mngr -v -b ${transport}://${broker_endp} -l ${log_filename} &
# Wait until dev_mngr has registred the signal handlers
sleep 1
# Signal dev_mngr of a "new" PCIe device
killall -SIGUSR1 dev_mngr

# Wait until worker has been created
sleep 1
# Change IPC permissions if transport = ipc
if [ "$transport" = "ipc" ]
then
    chmod 777 $broker_endp
fi
