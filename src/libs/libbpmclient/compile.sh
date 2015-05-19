#!/bin/bash

# All of our Makefile options

# Select board in which we will work. Options are: ml605 or afcv3
BOARD=afcv3
#Select if we want to compile with debug mode on. Options are: y(es) or n(o)
ERRHAND_DBG=y
# Select the minimum debug verbosity. See liberrhand file errhand_opts.h for more info.
ERRHAND_MIN_LEVEL=DBG_LVL_INFO
# Select the subsytems which will have the debug on. See liberrhand file errhand_opts.h for more info.
ERRHAND_SUBSYS_ON='"(DBG_DEV_MNGR | DBG_DEV_IO | DBG_SM_IO | DBG_LIB_CLIENT | DBG_SM_PR | DBG_SM_CH | DBG_LL_IO | DBG_HAL_UTILS)"'
# Select the FMC ADC board type. Options are: passive or active

COMMAND_LIBBPMCLIENT="\
    make BOARD=${BOARD} \
    ERRHAND_DBG=${ERRHAND_DBG} \
    ERRHAND_MIN_LEVEL=${ERRHAND_MIN_LEVEL} \
    ERRHAND_SUBSYS_ON='"${ERRHAND_SUBSYS_ON}"' && \
    sudo make install"

COMMAND_ARRAY=(
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

