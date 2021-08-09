#!/usr/bin/env bash

set -euo pipefail

# This must follow the format "<port_number>,<bitstream_name_without_extension>"
BOARDS="$1"
DCCS="${2:-3}"
NUM_GTS="${3:-8}"

SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

OLDIFS=$IFS; IFS=',';
# Separate "tuple" arguments with positional notation
set -- ${BOARDS};
for board in ${BOARDS[*]}; do
    IFS=$OLDIFS;

    printf "Configuring BPM board: %s\n" "${board}"
    for dcc in `seq 0 $((${DCCS}-1))`; do
        num_gts=${NUM_GTS}
        
        printf "DCC: %s\n" "${dcc}"
        (cd build/ && ./examples/fofb_ctrl --verbose --num_gts ${num_gts} --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber ${dcc} --bpm_id $((2*${board}-1)) --time_frame_len 5000 --cc_enable 0)

        (cd build/ && ./examples/fofb_ctrl --verbose --num_gts ${num_gts} --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber ${dcc} --cc_enable 1)
        # old gateware
        (cd build/ && ./examples/trigger --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber ${dcc} --channumber 2 --rcvsrc 0 --rcvsel 5)
        # new bpm gateware
        (cd build/ && ./examples/trigger --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber ${dcc} --channumber 20 --rcvsrc 0 --rcvsel 5)
    done;
done;
