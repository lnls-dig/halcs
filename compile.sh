#!/usr/bin/env bash

VALID_BOARDS_STR="Valid values are: \"ml605\", \"afcv3\" or \"afcv3_1\"."
VALID_APPS_STR="Valid values are: \"ebpm\"."
VALID_WITH_EXAMPLES_STR="Valid values are: \"yes\" or \"no\"."
VALID_WITH_LIBS_LINK_STR="Valid values are: \"yes\" or \"no\"."

function usage() {
    echo "Usage: $0 [-b <board>] [-a <applications>] [-e <with examples = yes/no>] [-l <with library linking = yes/no>] [-x <extra flags>]"
}

#######################################
# All of our Makefile options
#######################################

BOARD=
APPS=
WITH_EXAMPLES=
WITH_LIBS_LINK=
EXTRA_FLAGS=

# Get command line options
while getopts ":b:a:e:l:x:" opt; do
    case $opt in
        b)
            BOARD=$OPTARG
            ;;
        a)
            APPS=$OPTARG
            ;;
        e)
            WITH_EXAMPLES=$OPTARG
            ;;
        l)
            WITH_LIBS_LINK=$OPTARG
            ;;
        x)
            EXTRA_FLAGS=$OPTARG
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            usage
            exit 1
            ;;
        :)
            echo "Option -$OPTARG requires an argument." >&2
            usage
            exit 1
            ;;
    esac
done

if [ -z "$BOARD"  ]; then
    echo "\"board\" variable unset."
    usage
    exit 1
fi

if [ "$BOARD" != "afcv3" ] && [ "$BOARD" != "afcv3_1" ] && [ "$BOARD" != "ml605" ]; then
    echo "Unsupported board. "$VALID_BOARDS_STR
    exit 1
fi

if [ -z "$APPS"  ]; then
    echo "\"applications\" variable unset."
    usage
    exit 1
fi

if [ -z "$WITH_EXAMPLES"  ]; then
    echo "\"examples\" variable unset."
    usage
    exit 1
fi

if [ "$WITH_EXAMPLES" != "yes" ] && [ "$WITH_EXAMPLES" != "no" ]; then
    echo "Unsupported examples. "$VALID_WITH_EXAMPLES_STR
    exit 1
fi

if [ -z "$WITH_LIBS_LINK"  ]; then
    echo "\"library linking\" variable unset."
    usage
    exit 1
fi

if [ "$WITH_LIBS_LINK" != "yes" ] && [ "$WITH_LIBS_LINK" != "no" ]; then
    echo "Unsupported library linking. "$VALID_WITH_LIBS_LINK_STR
    exit 1
fi

#EXTRA_FLAGS=()
## Get all other arguments
#for item in "${@:5}"
#do
#    EXTRA_FLAGS+=("${item}")
#done

# Select if we want to have the AFCv3 DDR memory shrink to 2^28 or the full size 2^32. Options are: (y)es ot (n)o.
# This is a TEMPORARY fix until the AFCv3 FPGA firmware is fixed. If unsure, select (y)es.
SHRINK_AFCV3_DDR_SIZE=y
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
# Select the FMC ADC board type. Options are: passive or active
FMC250M_4CH_TYPE=passive
# Select if we should program FMC EEPROM with some code or not. Option are:
# active, passive or nothing (dont' program EEPROM)
FMC250M_4CH_EEPROM_PROGRAM=
# Selects if we want to compile DEV_MNGR. Options are: y(es) or n(o)
WITH_DEV_MNGR=y
# Selects the AFE RFFE version. Options are: 2
AFE_RFFE_TYPE=2
# Selects if we want to compile DEVIO Config. Options are: y(es) or n(o).
# If selected, the FPGA firmware must have the AFC diagnostics module
# synthesized.
WITH_APP_CFG=n
# Selects the install location of the config file
CFG_FILENAME=/etc/bpm_sw/bpm_sw.cfg
# Selects the install location of the config file
PREFIX=/usr/local
CFG_DIR=${PREFIX}/etc/bpm_sw
# Selects which config file to install. Options are: crude_defconfig or lnls_defconfig
CFG=lnls_defconfig
export CFG

COMMAND_DEPS="\
    make ${EXTRA_FLAGS[@]} deps && \
    make ${EXTRA_FLAGS[@]} deps_install"

COMMAND_LIBS="\
    make \
    ${EXTRA_FLAGS[@]} \
    BOARD=${BOARD} \
    ERRHAND_DBG=${ERRHAND_DBG} \
    ERRHAND_MIN_LEVEL=${ERRHAND_MIN_LEVEL} \
    ERRHAND_SUBSYS_ON='"${ERRHAND_SUBSYS_ON}"' \
    LOCAL_MSG_DBG=${LOCAL_MSG_DBG}  \
    libs_compile_install"

COMMAND_HAL="\
    make \
    ${EXTRA_FLAGS[@]} \
    BOARD=${BOARD} \
    APPS=${APPS} \
    SHRINK_AFCV3_DDR_SIZE=${SHRINK_AFCV3_DDR_SIZE} \
    ERRHAND_DBG=${ERRHAND_DBG} \
    ERRHAND_MIN_LEVEL=${ERRHAND_MIN_LEVEL} \
    ERRHAND_SUBSYS_ON='"${ERRHAND_SUBSYS_ON}"' \
    LOCAL_MSG_DBG=${LOCAL_MSG_DBG} \
    FMC130M_4CH_TYPE=${FMC130M_4CH_TYPE} \
    FMC130M_4CH_EEPROM_PROGRAM=${FMC130M_4CH_EEPROM_PROGRAM} \
    WITH_DEV_MNGR=${WITH_DEV_MNGR} \
    AFE_RFFE_TYPE=${AFE_RFFE_TYPE} \
    WITH_APP_CFG=${WITH_APP_CFG} \
    CFG_DIR=${CFG_DIR} && \
    make CFG=${CFG} ${EXTRA_FLAGS[@]} install"

if [ "$WITH_EXAMPLES" = "yes" ]; then
COMMAND_EXAMPLES="\
    make ${EXTRA_FLAGS[@]} examples"
else
COMMAND_EXAMPLES=""
fi

if [ "$WITH_LIBS_LINK" = "yes" ]; then
COMMAND_LIBS_LINK="ldconfig"
else
COMMAND_LIBS_LINK=""
fi

COMMAND_ARRAY=(
    "${COMMAND_DEPS}"
    "${COMMAND_LIBS}"
    "${COMMAND_HAL}"
    "${COMMAND_EXAMPLES}"
    "${COMMAND_LIBS_LINK}"
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

