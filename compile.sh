#!/bin/bash

VALID_BOARDS_STR="Valid values are: \"ml605\" and \"afcv3\"."

#######################################
# All of our Makefile options
#######################################

# Select board in which we will work. Options are: ml605 or afcv3
BOARD=$1

if [ -z "$BOARD" ]; then
    echo "\"BOARD\" variable unset. "$VALID_BOARDS_STR
    exit 1
fi

if [ "$BOARD" != "afcv3" ] && [ "$BOARD" != "ml605" ]; then
    echo "Unsupported board. "$VALID_BOARDS_STR
    exit 1
fi

#Select if we want to compile code with all messages outputs. Options are: y(es) or n(o)
LOCAL_MSG_DBG=n
#Select if we want to compile with debug mode on. Options are: y(es) or n(o)
ERRHAND_DBG=y
# Select the minimum debug verbosity. See liberrhand file errhand_opts.h for more info.
ERRHAND_MIN_LEVEL=DBG_LVL_TRACE
# Select the subsytems which will have the debug on. See liberrhand file errhand_opts.h for more info.
ERRHAND_SUBSYS_ON='"(DBG_DEV_MNGR | DBG_DEV_IO | DBG_SM_IO | DBG_LIB_CLIENT | DBG_SM_PR | DBG_SM_CH | DBG_LL_IO | DBG_HAL_UTILS)"'
# Select the FMC ADC board type. Options are: passive or active
FMC130M_4CH_TYPE=passive
# Select if we should program FMC EEPROM with some code or not. Option are:
# active, passive or nothing (dont' program EEPROM)
FMC130M_4CH_EEPROM_PROGRAM=
# Selects if we want to compile DEV_MNGR. Options are: y(es) or n(o)
WITH_DEV_MNGR=y
# Selects the AFE RFFE version. Options are: 2
AFE_RFFE_TYPE=2
# Selects if we want to compile DEVIO Config. Options are: y(es) or n(o).
# If selected, the FPGA firmware must have the AFC diagnostics module
# synthesized.
WITH_DEVIO_CFG=y
# Selects the install location of the config file
CFG_FILENAME=/etc/bpm_sw/bpm_sw.cfg
# Selects the install location of the config file
CFG_DIR=/etc/bpm_sw

COMMAND_DEPS="\
    make deps && \
    sudo make deps_install"

COMMAND_HAL="\
    make BOARD=${BOARD} \
    ERRHAND_DBG=${ERRHAND_DBG} \
    ERRHAND_MIN_LEVEL=${ERRHAND_MIN_LEVEL} \
    ERRHAND_SUBSYS_ON='"${ERRHAND_SUBSYS_ON}"' \
    LOCAL_MSG_DBG=${LOCAL_MSG_DBG} \
    FMC130M_4CH_TYPE=${FMC130M_4CH_TYPE} \
    FMC130M_4CH_EEPROM_PROGRAM=${FMC130M_4CH_EEPROM_PROGRAM} \
    WITH_DEV_MNGR=${WITH_DEV_MNGR} \
    AFE_RFFE_TYPE=${AFE_RFFE_TYPE} \
    WITH_DEVIO_CFG=${WITH_DEVIO_CFG} \
    CFG_DIR=${CFG_DIR} && \
    sudo make install"

COMMAND_LIBBPMCLIENT="\
    make BOARD=${BOARD} \
    ERRHAND_DBG=${ERRHAND_DBG} \
    ERRHAND_MIN_LEVEL=${ERRHAND_MIN_LEVEL} \
    ERRHAND_SUBSYS_ON='"${ERRHAND_SUBSYS_ON}"' \
    LOCAL_MSG_DBG=${LOCAL_MSG_DBG} && \
    sudo make libbpmclient_install"

COMMAND_ARRAY=(
    "${COMMAND_DEPS}"
    "${COMMAND_HAL}"
    "${COMMAND_LIBBPMCLIENT}"
)

for i in "${COMMAND_ARRAY[@]}"
do
    echo "Executing: " $i
    eval $i

    # Check return value
    rc=$?
    if [[ $rc != 0 ]]; then
        exit $rc
    fi
done

