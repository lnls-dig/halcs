#!/usr/bin/env bash

set -u
set -e
set -a

CURDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# More general names first
${CURDIR}/change-proj-name.sh -o LIBBPMCLIENT -n LIBHALCSCLIENT -i change-bpm-name.sh
${CURDIR}/change-proj-name.sh -o libbpmclient -n libhalcsclient -i change-bpm-name.sh
${CURDIR}/change-proj-name.sh -o BPMCLIENT -n HALCSCLIENT -i change-bpm-name.sh
${CURDIR}/change-proj-name.sh -o bpmclient -n halcsclient -i change-bpm-name.sh
${CURDIR}/change-proj-name.sh -o BPM_CLIENT -n HALCS_CLIENT -i change-bpm-name.sh
${CURDIR}/change-proj-name.sh -o bpm_client -n halcs_client -i change-bpm-name.sh

${CURDIR}/change-proj-name.sh -o BPM-SW -n HALCS -i change-bpm-name.sh
${CURDIR}/change-proj-name.sh -o bpm-sw -n halcs -i change-bpm-name.sh
${CURDIR}/change-proj-name.sh -o "BPM_SW" -n 'HALCS' -i change-bpm-name.sh
${CURDIR}/change-proj-name.sh -o "bpm_sw" -n 'halcs' -i change-bpm-name.sh
${CURDIR}/change-proj-name.sh -o EBPM -n HALCSD -i change-bpm-name.sh
${CURDIR}/change-proj-name.sh -o ebpm -n halcsd -i change-bpm-name.sh

${CURDIR}/change-proj-name.sh -o "BPM" -n 'HALCS' -i change-bpm-name.sh
${CURDIR}/change-proj-name.sh -o "bpm" -n 'halcs' -i change-bpm-name.sh
