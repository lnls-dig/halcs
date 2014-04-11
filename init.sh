#!/bin/bash

# Broker Endpoint
EXPECTED_ARGS=1

if [ "$(id -u)" != "0" ]
then
	echo "This must be run as root"
	exit 1
fi

if [ $# -ne $EXPECTED_ARGS ]
then
	echo "Usage: `basename $0` {broker endpoint}"
	exit 1;
fi

broker_endp=$1

# Launch Device Manager 
./dev_mngr "ipc://"$broker_endp &
# Signal dev_mngr of a "new" PCIe device
killall -SIGUSR1 dev_mngr

# Change IPC permissions
chmod 777 $broker_endp
