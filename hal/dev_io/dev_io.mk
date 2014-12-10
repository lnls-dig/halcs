dev_io_DIR = hal/dev_io

# Here we call <output_name>_core_OBJS as we need to add
# more objects to this target. This is done in the hal.mk
# makefile
dev_io_core_OBJS = $(dev_io_DIR)/dev_io_core.o \
		   $(dev_io_DIR)/dev_io_err.o

# Compile Digital Back-End DEVIO
ifeq ($(WITH_DBE_DEVIO),y)
dev_io_be_OBJS = $(dev_io_DIR)/dev_io_be.o
dev_io_be_OUT = dev_io_be
else
dev_io_be_OBJS =
dev_io_be_OUT =
endif

# Compile Analog Front-End DEVIO
ifeq ($(WITH_AFE_DEVIO),y)
dev_io_fe_OBJS = $(dev_io_DIR)/dev_io_fe.o
dev_io_fe_OUT = dev_io_fe
else
dev_io_fe_OBJS =
dev_io_fe_OUT =
endif

dev_io_OBJS = $(dev_io_be_OBJS) $(dev_io_fe_OBJS)
dev_io_INCLUDE_DIRS = $(dev_io_DIR) $(LIBCLIENT_DIR)

# All possible DEVIO objects. Used for cleaning
dev_io_all_OUT = dev_io_be dev_io_fe

# Specified target DEVIOs
dev_io_OUT = $(dev_io_be_OUT) $(dev_io_fe_OUT)
