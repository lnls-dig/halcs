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
# Selects the base IP address of all AFE RFFE. The AFE RFFE must be located
# starting to the specified IP address up to the number of the AFE RFFE.
#
# Example:
#
# if AFE_BASE_IP_ADDR = 192.168.0.%u and AFE_BASE_IP_OFFSET = 100, the AFE
# RFFE would have the following IP addresses:
#
# AFE RFFE #1 (DBE #1): 192.168.0.100
# AFE RFFE #2 (DBE #1): 192.168.0.101
# AFE RFFE #3 (DBE #2): 192.168.0.102
# AFE RFFE #4 (DBE #2): 192.168.0.103
# AFE RFFE #3 (DBE #3): 192.168.0.104
# AFE RFFE #4 (DBE #3): 192.168.0.105
# ...
AFE_TRANSPORT="tcp"
AFE_BASE_IP_PORT="6791"
AFE_BASE_IP_PATTERN="10.0.18.%u"
AFE_BASE_IP_OFFSET=100

COMMAND="\
    make BOARD=${BOARD} \
    LOCAL_MSG_DBG=${LOCAL_MSG_DBG} \
    DBE_DBG=${DBE_DBG} \
    FMC130M_4CH_TYPE=${FMC130M_4CH_TYPE} \
    FMC130M_4CH_EEPROM_PROGRAM=${FMC130M_4CH_EEPROM_PROGRAM} \
    WITH_DEV_MNGR=${WITH_DEV_MNGR} \
    AFE_RFFE_TYPE=${AFE_RFFE_TYPE} \
    AFE_TRANSPORT=${AFE_TRANSPORT} \
    AFE_BASE_IP_PATTERN=${AFE_BASE_IP_PATTERN} \
    AFE_BASE_IP_OFFSET=${AFE_BASE_IP_OFFSET} \
    && sudo make install"

echo "Executing: " ${COMMAND}
eval ${COMMAND}
