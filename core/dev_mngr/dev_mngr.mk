dev_mngr_DIR = $(SRC_DIR)/core/dev_mngr/src/dev_mngr/c

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

dev_mngr_all_OUT = dev_mngr

dev_mngr_LIBS = -lbsmp
dev_mngr_STATIC_LIBS =
