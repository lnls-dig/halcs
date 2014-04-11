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

sudo valgrind --leak-check=yes --trace-children=yes \
	--suppressions=valgrind.supp ./dev_mngr "ipc://"$broker_endp > \
	valgrind_report.txt 2>&1
