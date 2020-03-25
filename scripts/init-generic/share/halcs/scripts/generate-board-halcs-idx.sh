#!/bin/sh

INPUT=$1

INSTANCE_IDX=$(echo ${INPUT} | sed 's|.*-||g')
BOARD_IDX=$(expr ${INSTANCE_IDX} / 2 + ${INSTANCE_IDX} % 2)
HALCS_IDX=$(expr 1 - ${INSTANCE_IDX} % 2)

# Output space-separated indexes
echo ${INSTANCE_IDX} ${BOARD_IDX} ${HALCS_IDX}
