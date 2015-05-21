#!/usr/bin/env bash

USER=$(whoami)
IP="localhost"
PREFIX=$(pwd)
DIR="bpm-sw-deploy"
DIST_NAME="bpm-sw"
DEFAULT_BOARD="afcv3"
VALID_BOARDS_STR="Valid values are: \"ml605\" and \"afcv3\"."

# Select board in which we will work. Options are: ml605 or afcv3
BOARD_TYPE=$1

if [ -z "$BOARD_TYPE" ]; then
    echo "\"BOARD_TYPE\" variable unset. "$VALID_BOARDS_STR
    exit 1
fi

if [ "$BOARD_TYPE" != "afcv3" ] && [ "$BOARD_TYPE" != "ml605" ]; then
    echo "Unsupported board. "$VALID_BOARDS_STR
    exit 1
fi

./deploy.sh ${USER} ${IP} ${PREFIX}/${DIR}
~/makeself-2.2.0/makeself.sh --bzip2 ${PREFIX}/${DIR} ${DIST_NAME}.bz2.run "LNLS BPM Software Package" \
    ./compile.sh ${BOARD_TYPE}
