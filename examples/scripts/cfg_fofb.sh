#!/usr/bin/env bash

set -euo pipefail

# This must follow the format "<port_number>,<bitstream_name_without_extension>"
BOARDS="$1"

SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

OLDIFS=$IFS; IFS=',';
# Separate "tuple" arguments with positional notation
set -- ${BOARDS};
for board in ${BOARDS[*]}; do
    printf "Configuring BPM board: %s\n" "${board}"
    (cd build/ && ./examples/fofb_ctrl --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber 0 --bpm_id ${board} --time_frame_len 10000 --cc_enable 0)
    (cd build/ && ./examples/fofb_ctrl --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber 1 --bpm_id ${board} --time_frame_len 10000 --cc_enable 0)
    (cd build/ && ./examples/fofb_ctrl --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber 0 --cc_enable 1)
    (cd build/ && ./examples/fofb_ctrl --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber 1 --cc_enable 1)
    (cd build/ && ./examples/trigger --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber 0 --channumber 1 --rcvsrc 0 --rcvsel 3)
    (cd build/ && ./examples/trigger --brokerendp ipc:///tmp/malamute --boardslot ${board} --halcsnumber 1 --channumber 1 --rcvsrc 0 --rcvsel 3)
done;

IFS=$OLDIFS;
