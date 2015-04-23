dev_mngr_DIR = $(SRC_DIR)/dev_mngr

# Here we call <output_name>_core_OBJS as we need to add
# more objects to this target. This is done in the hal.mk
# makefile
dev_mngr_core_OBJS = $(dev_mngr_DIR)/dev_mngr_core.o \
		     $(dev_mngr_DIR)/dev_mngr_err.o \
		     $(dev_mngr_DIR)/dev_mngr_dev_info.o

# Compile Digital Back-End DEVIO
ifeq ($(WITH_DEV_MNGR),y)
dev_mngr_OBJS = $(dev_mngr_DIR)/dev_mngr.o
dev_mngr_OUT = dev_mngr
else
dev_mngr_OBJS =
dev_mngr_OUT =
endif

dev_mngr_ALL_DIRS = $(dev_mngr_DIR)

# Add -I prefix to all include directories
dev_mngr_INCLUDE_DIRS = $(addprefix -I, $(dev_mngr_ALL_DIRS))

dev_mngr_all_OUT = dev_mngr
