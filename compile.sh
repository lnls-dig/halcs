#!/bin/bash

# All of our Makefile options

# Select board in which we will work. Options are: ml605 or afcv3
BOARD=afcv3
#Select if we want to compile code with all messages outputs. Options are: y(es) or n(o)
LOCAL_MSG_DBG=n
#Select if we want to compile with debug mode on. Options are: y(es) or n(o)
DBE_DBG=y
# Select the FMC ADC board type. Options are: passive or active
FMC130M_4CH_TYPE=passive
# Select if we should program FMC EEPROM with some code or not. Option are:
# active, passive or nothing (dont' program EEPROM)
FMC130M_4CH_EEPROM_PROGRAM=
# Selects if we want to compile DEV_MNGR. Options are: y(es) or n(o)
WITH_DEV_MNGR=y
# Selects the AFE RFFE version. Options are: 2
AFE_RFFE_TYPE=2
# Selects the install location of the config file
CFG_FILENAME=/etc/bpm_sw/bpm_sw.cfg
# Selects the install location of the config file
CFG_DIR=/etc/bpm_sw

COMMAND="\
    make BOARD=${BOARD} \
    LOCAL_MSG_DBG=${LOCAL_MSG_DBG} \
    DBE_DBG=${DBE_DBG} \
    FMC130M_4CH_TYPE=${FMC130M_4CH_TYPE} \
    FMC130M_4CH_EEPROM_PROGRAM=${FMC130M_4CH_EEPROM_PROGRAM} \
    WITH_DEV_MNGR=${WITH_DEV_MNGR} \
    AFE_RFFE_TYPE=${AFE_RFFE_TYPE} \
    CFG_DIR=${CFG_DIR} \
    && sudo make install"

echo "Executing: " ${COMMAND}
eval ${COMMAND}
