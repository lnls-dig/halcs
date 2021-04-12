#!/usr/bin/env bash

set -euo pipefail

SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

BOARD="${1:-afcv3_1}"
GENERATOR="${2:-RPM}"

mkdir -p build && \
cd build && \
cmake3 -Dcpack_generator_OPT=${GENERATOR} -Dcpack_components_grouping_OPT=ONE_PER_GROUP -Dcpack_components_all_OPT="Pciedriver" ../ && \
make package && \
cmake3 -Dhalcs_BOARD_OPT=${BOARD} -Dcpack_generator_OPT=${GENERATOR} -Dcpack_components_grouping_OPT=ALL_COMPONENTS_IN_ONE -Dcpack_components_all_OPT="Binaries;Libs;Scripts;Tools" ../ && \
make package && \
( [ "${GENERATOR}" = "RPM" ] && rpm -e halcsd || dpkg -r halcsd ); \
( [ "${GENERATOR}" = "RPM" ] && rpm -e halcsd-debuginfo || dpkg -r halcsd-debuginfo ); \
( [ "${GENERATOR}" = "RPM" ] && rpm -i halcsd-debuginfo* || dpkg -i halcsd-debuginfo* ); \
( [ "${GENERATOR}" = "RPM" ] && rpm -i halcsd-afcv3_1_*_x86_64.rpm || dpkg -i halcsd-afcv3_1_*_x86_64.deb) && \
(chmod 777 /tmp/malamute || :) && \
ldconfig && \
systemctl daemon-reload
