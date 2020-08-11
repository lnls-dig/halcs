#!/usr/bin/env bash

DEVICE_NUMBER=$1
GEN_IDX_SCRIPT_NAME="generate-dev-idx.sh"
DIR=$(dirname $(realpath $0))

# Find script location
gen_idx_script=$(${DIR}/find-script.sh ${GEN_IDX_SCRIPT_NAME})

HALCS_IDXS=($(${gen_idx_script} ${DEVICE_NUMBER} | awk '{for (i=2; i<NF; i++) printf $i " "; print $NF}'))
echo ${HALCS_IDXS[@]}
