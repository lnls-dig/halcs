#!/bin/bash

if [ "$(id -u)" != "0" ]
then
	echo "This must be run as root"
	exit 1
fi

killall mdp_broker
killall dev_io
killall dev_mngr

