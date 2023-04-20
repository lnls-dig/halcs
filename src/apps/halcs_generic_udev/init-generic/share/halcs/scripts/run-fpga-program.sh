#!/usr/bin/env bash

set -u

DIR=$(dirname $(realpath $0))

# This script it to be used with: "script_name" <gateware_name> \
#    <device_number> \
#    <fmc0_name> <fmc1_name> ... <fmcn_name>
GATEWARE_NAME=$1
DEVICE_NUMBER=$2
shift 2
FMC_NAMES=($@)

# Get HALCS instances
HALCS_IDXS=($(${DIR}/get-halcs-idxs.sh ${DEVICE_NUMBER}))

for i in $(seq 1 "${#HALCS_IDXS[@]}"); do
    prog_inst=$((i-1));
    case "${GATEWARE_NAME}" in
        bpm-gw*)
            case "${FMC_NAMES[$prog_inst]}" in
                LNLS_FMC250M*)
                    START_PROGRAM="/usr/bin/systemctl --no-block start halcs-ioc@${HALCS_IDXS[$prog_inst]}.target"
                    ;;
                LNLS_FMC130M*)
                    START_PROGRAM="/usr/bin/systemctl --no-block start halcs-ioc@${HALCS_IDXS[$prog_inst]}.target"
                    ;;
                *)
                    echo "Unsupported Gateware Module: "${FPGA_FMC_NAME} >&2
                    exit 1
                    ;;
            esac
            ;;

        tim-receiver*)
            START_PROGRAM="/usr/bin/systemctl --no-block start halcs-ioc@${HALCS_IDXS[$prog_inst]}.target"
            ;;

        afc-tim*)
            # Only start IOCs for even-numbered instances, as there is no device for odd-numbered instances
            if [ $((prog_inst%2)) -eq 0 ]; then
                START_PROGRAM="/usr/bin/systemctl --no-block start tim-rx-ioc@${HALCS_IDXS[$prog_inst]}.service"
            else
                START_PROGRAM=""
            fi
            ;;

        pbpm-gw*)
            START_PROGRAM="/usr/bin/systemctl --no-block start halcs-ioc@${HALCS_IDXS[$prog_inst]}.target"
            ;;

        *)
            # Only start HALCSs for even-numbered instances, as there is no device for odd-numbered instances
            if [ $((prog_inst%2)) -eq 0 ]; then
                START_PROGRAM="/usr/bin/systemctl --no-block start halcs-be@${HALCS_IDXS[$prog_inst]}.service"
            else
                START_PROGRAM=""
            fi
            ;;
    esac

    eval ${START_PROGRAM}
done
