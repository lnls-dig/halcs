#!/bin/env bash

set -a
set -e
set -u

BOARD_INIT=$1
BOARD_END=$2

for i in `seq ${BOARD_INIT} ${BOARD_END}`; do 
    for j in `seq 0 1`; do 
        echo "acq $i $j: "; 
        /root/postinstall/apps/bpm-app/halcs/examples/build/exe/acq/production/afcv3_1/acq -b \
            ipc:///tmp/malamute -o $i -s $j -c 0 -n 10 -f 0; 
    done;
done
