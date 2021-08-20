#!/usr/bin/env bash

VALID_APPS_STR="Valid values are: \"halcsd\", \"halcs_generic_udev\"."
VALID_WITH_EXAMPLES_STR="Valid values are: \"yes\" or \"no\"."
VALID_WITH_SYSTEM_INTEGRATION_STR="Valid values are: \"yes\" or \"no\"."
VALID_WITH_DRIVER_STR="Valid values are: \"yes\" or \"no\"."
VALID_WITH_HALCS_STR="Valid values are: \"yes\" or \"no\"."

function usage() {
    echo "Usage: $0 [-a <applications>] [-e <with examples = yes/no>]"
    echo "    [-l <with system integration = yes/no>] [-d <with driver = yes/no>]"
    echo "    [-f <with HALCS = yes/no>] [-x <extra flags>]"
}

LIBSDIR="libs"
SRCDIR="src"

#######################################
# All of our Makefile options
#######################################

APPS=
WITH_EXAMPLES="no"
WITH_SYSTEM_INTEGRATION="no"
WITH_DRIVER="no"
WITH_HALCS="yes"
EXTRA_FLAGS=

# Get command line options
while getopts ":a:e:l:x:d:f:" opt; do
    case $opt in
        a)
            APPS=$OPTARG
            ;;
        e)
            WITH_EXAMPLES=$OPTARG
            ;;
        l)
            WITH_SYSTEM_INTEGRATION=$OPTARG
            ;;
        x)
            EXTRA_FLAGS=$OPTARG
            ;;
        d)
            WITH_DRIVER=$OPTARG
            ;;
        f)
            WITH_HALCS=$OPTARG
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

if [ -z "$WITH_SYSTEM_INTEGRATION"  ]; then
    echo "\"system integration\" variable unset."
    usage
    exit 1
fi

if [ "$WITH_SYSTEM_INTEGRATION" != "yes" ] && [ "$WITH_SYSTEM_INTEGRATION" != "no" ]; then
    echo "Unsupported system integration option. "$VALID_WITH_SYSTEM_INTEGRATION_STR
    exit 1
fi

if [ "$WITH_DRIVER" != "yes" ] && [ "$WITH_DRIVER" != "no" ]; then
    echo "Unsupported driver option. "$VALID_WITH_DRIVER_STR
    exit 1
fi

if [ "$WITH_HALCS" != "yes" ] && [ "$WITH_HALCS" != "no" ]; then
    echo "Unsupported driver option. "$VALID_WITH_HALCS_STR
    exit 1
fi

# Select if we want to have the AFCv3 DDR memory shrink to 2^28 or the full size 2^32. Options are: (y)es ot (n)o.
# This is a TEMPORARY fix until the AFCv3 FPGA firmware is fixed. If unsure, select (y)es.
SHRINK_AFCV3_DDR_SIZE=n
#Select if we want to compile code with all messages outputs. Options are: y(es) or n(o)
LOCAL_MSG_DBG=n
#Select if we want to compile with debug mode on. Options are: y(es) or n(o)
ERRHAND_DBG=y
# Select the minimum debug verbosity. See liberrhand file errhand_opts.h for more info.
ERRHAND_MIN_LEVEL=DBG_LVL_WARN
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

# Install HALCS only if requested
if [ "$WITH_HALCS" = "yes" ]; then
    COMMAND_DEPS="\
        make ${EXTRA_FLAGS} deps \
        && sudo make ${EXTRA_FLAGS} deps_install"

    COMMAND_LIBS="\
        make \
        ${EXTRA_FLAGS} \
        ERRHAND_DBG=${ERRHAND_DBG} \
        ERRHAND_MIN_LEVEL=${ERRHAND_MIN_LEVEL} \
        ERRHAND_SUBSYS_ON='"${ERRHAND_SUBSYS_ON}"' \
        LOCAL_MSG_DBG=${LOCAL_MSG_DBG} \
        && sudo make \
        ${EXTRA_FLAGS} \
        ERRHAND_DBG=${ERRHAND_DBG} \
        ERRHAND_MIN_LEVEL=${ERRHAND_MIN_LEVEL} \
        ERRHAND_SUBSYS_ON='"${ERRHAND_SUBSYS_ON}"' \
        LOCAL_MSG_DBG=${LOCAL_MSG_DBG} \
        libs_compile_install"

    COMMAND_CORE="\
        make \
        ${EXTRA_FLAGS} \
        APPS='"${APPS}"' \
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
        && sudo make \
        ${EXTRA_FLAGS} \
        APPS='"${APPS}"' \
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
        core_install"

    if [ "$WITH_EXAMPLES" = "yes" ]; then
    COMMAND_EXAMPLES="\
        make ${EXTRA_FLAGS} examples"
    else
    COMMAND_EXAMPLES=""
    fi
else
    COMMAND_DEPS=""
    COMMAND_LIBS=""
    COMMAND_CORE=""
    COMMAND_EXAMPLES=""
fi

if [ "$WITH_HALCS" = "yes" ]; then
    if [ "$WITH_SYSTEM_INTEGRATION" = "yes" ]; then
        COMMAND_SYSTEM_INTEGRATION="\
            sudo make
            APPS='"${APPS}"' \
            scripts_install && \
            sudo ldconfig"
    else
        COMMAND_SYSTEM_INTEGRATION=""
    fi
fi

if [ "$WITH_DRIVER" = "yes" ]; then
    COMMAND_DRIVER="\
        make ${EXTRA_FLAGS} pcie_driver && \
        sudo make ${EXTRA_FLAGS} pcie_driver_install"
else
    COMMAND_DRIVER=""
fi

if [ "$WITH_HALCS" = "yes" ]; then
    # Meta target to remove copied headers
    COMMAND_CLEAN="make -C ${LIBSDIR}/acqclient pre_clean && \
        make -C ${LIBSDIR}/bpmclient pre_clean &&
        make -C ${SRCDIR}/client pre_clean"
else
    COMMAND_CLEAN=""
fi

COMMAND_ARRAY=(
    "${COMMAND_DEPS}"
    "${COMMAND_DRIVER}"
    "${COMMAND_LIBS}"
    "${COMMAND_CORE}"
    "${COMMAND_EXAMPLES}"
    "${COMMAND_SYSTEM_INTEGRATION}"
    "${COMMAND_CLEAN}"
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
