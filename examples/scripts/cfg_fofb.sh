#!/usr/bin/env bash

set -euo pipefail

# This must follow the format "<port_number>,<bitstream_name_without_extension>"
BOARDS="$1"
DCCS="${2:-3}"
NUM_GTS="${3:-8}"
CRATE_NUM=$4

SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

OLDIFS=$IFS; IFS=',';

FOFB_CTRLS_OFFS=480

# Separate "tuple" arguments with positional notation
set -- ${BOARDS};
for board in ${BOARDS[*]}; do
    IFS=$OLDIFS;

    printf "Configuring BPM board: %s\n" "${board}"
    for dcc in `seq 0 $((${DCCS}-1))`; do
        num_gts=${NUM_GTS}

        if [ ${board} -eq 2 ]; then
          # FOFB Controllers' IDs range from 480 to 499
          bpm_id=$((${FOFB_CTRLS_OFFS} + (${CRATE_NUM}-1)))
        else
          # BPMs' IDs range from 0 to 159
          bpm_id=$((8*(${CRATE_NUM}-1) + (2*(${board}-7))))
        fi

        printf "DCC: %s\n" "${dcc}"
        (fofb_ctrl --verbose --num_gts ${num_gts} --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber ${dcc} --bpm_id ${bpm_id} --time_frame_len 5000 --cc_enable 0)
        (fofb_ctrl --verbose --num_gts ${num_gts} --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber ${dcc} --cc_enable 1)

        if [ ${board} -ne 2 ]; then
          # old bpm gateware
          (trigger --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber ${dcc} --channumber 1 --rcvsrc 0 --rcvsel 5)
          (trigger --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber ${dcc} --channumber 2 --rcvsrc 0 --rcvsel 5)
          # new bpm gateware
          (trigger --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber ${dcc} --channumber 20 --rcvsrc 0 --rcvsel 5)
        fi
    done;
done;
