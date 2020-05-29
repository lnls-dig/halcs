#!/usr/bin/env bash

export PATH=/sbin:/bin:/usr/sbin:/usr/bin
DEVICE_PREFIX=/dev
EAT_DASHES_SCRIPT_NAME="eat-dashes.sh"
DIR=$(dirname $(realpath $0))

# Default values
DEVICE_NUMBER=""
DEVICE_NAME="/dev/fpga-"
DEVICE_ID=""
SDB_ENTRY_POINT_OFFSET="0x0"

while [ "$#" -gt 0 ]; do
    case "$1" in
        "-n"|"--device-number") DEVICE_NUMBER="$2" ;;
        "-d"|"--device-name") DEVICE_NAME="$2" ;;
        "-i"|"--device-id") DEVICE_ID="$2" ;;
        "-e"|"--sdb-entry-point") SDB_ENTRY_POINT_OFFSET="$2" ;;
        *) echo "Usage:" >&2
            echo "  $0 -n DEVICE_NUMBER [-d DEVICE_NAME] [-e SDB_ENTRY_POINT_OFFSET] " >&2
            echo >&2
            echo " Options:" >&2
            echo "  -n or --device-number    Specify device number to parse SDB gateware name = [1-12]" >&2
            echo "  -d or --device-name      Specify device name prefix to use for parsing SDB gateware name [default = \"/dev/fpga-\"]" >&2
            echo "  -i or --device-id        Specify the full device identifier for parsing SDB gateware name [this overrides -n and -d]" >&2
            echo "  -e or --sdb-entry-point  Specify SDB entry point offset [default = 0x0]" >&2
            exit 1
    esac

    shift 2
done

if [ -z "${DEVICE_ID}" ] && [ -z "${DEVICE_NUMBER}" ]; then
    echo "DEVICE_ID and DEVICE_NUMBER not set. Please use -n or -i option" >&2
    exit 2
fi

# if DEVICE_ID is not set, create the name by concatenating
# DEVICE_NAME and DEVICE_NUMBER
if [ -z "${DEVICE_ID}" ]; then
  DEVICE_ID=${DEVICE_NAME}${DEVICE_NUMBER}
else
  eat_dashes_script=$(${DIR}/find-script.sh ${EAT_DASHES_SCRIPT_NAME})
  DEVICE_NUMBER=$(${eat_dashes_script} ${DEVICE_ID})
fi

# Get HALCS instances
HALCS_IDXS=($(${DIR}/get-halcs-idxs.sh ${DEVICE_NUMBER}))

FULL_DEVICE_NAME=${DEVICE_PREFIX}/${DEVICE_ID}
# Check if device file is opened. If it is we must close it before
# probing for SDB gateware name
LSOF_OUT=$(/sbin/lsof ${FULL_DEVICE_NAME})
if [ ! -z "${LSOF_OUT}" ]; then
    for halcs_intance in "${HALCS_IDXS[@]}"; do
        systemctl stop halcs@${halcs_intance}.target
    done
fi

# Now, safely probe the SDB gateware name.
SDB_PARSER_NAME="sdb-read-lnls"

# Our SDB parser should be in PATH, but let's check anyway
sdb_parser=$(realpath $(which "${SDB_PARSER_NAME}" | head -n 1))
if [ -z "${sdb_parser}" ]; then
    # Try finding the script at some other default locations
    sdb_parser=$(${DIR}/find-script.sh ${SDB_PARSER_NAME})
fi

if [ -z "${sdb_parser}" ]; then
    echo "Could not find script: ${SDB_PARSER_NAME}" >&2
    exit 4
fi

# Finally, the SDB gateware name
SDB_OUTPUT=$(${sdb_parser} -l -e ${SDB_ENTRY_POINT_OFFSET} ${FULL_DEVICE_NAME} 2>/dev/null)
GATEWARE_NAME=$(echo "${SDB_OUTPUT}" | \
    grep "synthesis-name:" | \
    head -n 1 | \
    awk '{print $2}'
)
FMC_NAMES=($(echo "${SDB_OUTPUT}" | \
    grep "FMC" | \
    while read line; do
        echo "$line" | awk '{print $4}'
    done
))

echo ${GATEWARE_NAME} ${DEVICE_NUMBER} ${FMC_NAMES[@]}
