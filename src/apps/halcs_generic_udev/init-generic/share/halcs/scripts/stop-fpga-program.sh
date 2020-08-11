#!/usr/bin/env bash

set -u

DIR=$(dirname $(realpath $0))

# This script it to be used with: "script_name" <device instance> \
#    <device_number 1> <device number 2> ... <device number n>
DEV_INSTANCE=$1
shift 1
DEV_NUMBERS=($@)

for i in "${DEV_NUMBERS[@]}"; do
    STOP_PROGRAM="/usr/bin/systemctl --no-block stop halcs@${i}.target"

    eval ${STOP_PROGRAM}
done
