#!/usr/bin/env bash

set -euo pipefail

SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

GENERATOR="${1:-RPM}"

mkdir -p build
cd build
cmake3 \
    -Dcpack_generator_OPT=${GENERATOR} \
    -Dcpack_components_grouping_OPT=ONE_PER_GROUP \
    -Dcpack_components_all_OPT="Pciedriver" \
    ../
make package
cmake3 \
    -Dcpack_generator_OPT=${GENERATOR} \
    -Dcpack_components_grouping_OPT=ALL_COMPONENTS_IN_ONE \
    -Dcpack_components_all_OPT="Binaries;Libs;Scripts;Tools" \
    ../
make package

case "${GENERATOR}" in
    RPM)
        rpm -e halcsd || :
        rpm -e halcsd-debuginfo || :
        rpm -i halcsd-debuginfo*
        rpm -i halcsd*_x86_64.rpm 
    ;;
    DEB)
        dpkg -r halcsd || :
        dpkg -r halcsd-debuginfo || :
        dpkg -i halcsd-debuginfo*
        dpkg -i halcsd-debuginfo*
    ;;
    *)
        echo "Invalid Gateware: "${FPGA_SYNTHESIS_NAME} >&2
        exit 1
        ;;
esac

chmod 777 /tmp/malamute || :
ldconfig
systemctl daemon-reload
udevadm control -R
