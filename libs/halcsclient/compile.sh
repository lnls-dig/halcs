#!/usr/bin/env bash

VALID_BOARDS_STR="Valid values are: \"ml605\", \"afcv3\" or \"afcv3_1\"."

#######################################
# All of our Makefile options
#######################################

# Select board in which we will work. Options are: ml605 or afcv3
BOARD=$1

if [ -z "$BOARD" ]; then
    echo "\"BOARD\" variable unset. "$VALID_BOARDS_STR
    exit 1
fi

if [ "$BOARD" != "afcv3" ] && [ "$BOARD" != "afcv3_1" ] && [ "$BOARD" != "ml605" ]; then
    echo "Unsupported board. "$VALID_BOARDS_STR
    exit 1
fi

EXTRA_FLAGS=$2

#Select if we want to compile with debug mode on. Options are: y(es) or n(o)
ERRHAND_DBG=y
# Select the minimum debug verbosity. See liberrhand file errhand_opts.h for more info.
ERRHAND_MIN_LEVEL=DBG_LVL_INFO
# Select the subsytems which will have the debug on. See liberrhand file errhand_opts.h for more info.
ERRHAND_SUBSYS_ON='"(DBG_DEV_MNGR | DBG_DEV_IO | DBG_SM_IO | DBG_LIB_CLIENT | DBG_SM_PR | DBG_SM_CH | DBG_LL_IO | DBG_HAL_UTILS)"'
# Select the FMC ADC board type. Options are: passive or active

COMMAND_LIBHALCSCLIENT="\
    make \
    ${EXTRA_FLAGS} \
    BOARD=${BOARD} \
    ERRHAND_DBG=${ERRHAND_DBG} \
    ERRHAND_MIN_LEVEL=${ERRHAND_MIN_LEVEL} \
    ERRHAND_SUBSYS_ON='"${ERRHAND_SUBSYS_ON}"' && \
    sudo make install"

COMMAND_ARRAY=(
    "${COMMAND_LIBHALCSCLIENT}"
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

