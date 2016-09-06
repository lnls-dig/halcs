#!/bin/sh

INPUT=$1

DEV_IDX=$(echo ${INPUT} | sed 's|.*-||g')
HALCS_IDX=$(expr ${DEV_IDX} \* 2 - 1)

# Output space-separated indexes
echo ${DEV_IDX} ${HALCS_IDX}
