#!/usr/bin/env bash

set -u
set -e
set -a

CURDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# More general names first
${CURDIR}/change-proj-name.sh -o LIBBPMCLIENT -n LIBHALCSCLIENT
${CURDIR}/change-proj-name.sh -o libbpmclient -n libhalcsclient
${CURDIR}/change-proj-name.sh -o BPMCLIENT -n HALCSCLIENT
${CURDIR}/change-proj-name.sh -o bpmclient -n halcsclient
${CURDIR}/change-proj-name.sh -o BPM_CLIENT -n HALCS_CLIENT
${CURDIR}/change-proj-name.sh -o bpm_client -n halcs_client

${CURDIR}/change-proj-name.sh -o BPM-SW -n HALCS
${CURDIR}/change-proj-name.sh -o bpm-sw -n halcs
${CURDIR}/change-proj-name.sh -o BPM_SW -n HALCS
${CURDIR}/change-proj-name.sh -o bpm_sw -n halcs
${CURDIR}/change-proj-name.sh -o EBPM -n HALCSD
${CURDIR}/change-proj-name.sh -o ebpm -n halcsd

${CURDIR}/change-proj-name.sh -o BPM_ -n HALCS_
${CURDIR}/change-proj-name.sh -o bpm_ -n halcs_
