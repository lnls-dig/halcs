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
	echo "Usage: `basename $0` {cfg file}"
	exit 1;
fi

cfg_file=$1

valgrind --leak-check=yes --trace-children=yes \
	--suppressions=valgrind.supp ./dev_mngr -f $cfg_file > \
	valgrind_report.txt 2>&1
