include hal/dev_io/utils/utils.mk

dev_io_DIR = hal/dev_io

# Here we call <output_name>_core_OBJS as we need to add
# more objects to this target. This is done in the hal.mk
# makefile
dev_io_core_OBJS = $(dev_io_DIR)/dev_io_core.o \
		   $(dev_io_DIR)/dev_io_err.o \
    	   $(dev_io_utils_OBJS)

dev_io_OBJS = $(dev_io_DIR)/dev_io.o

dev_io_cfg_OBJS = $(dev_io_DIR)/dev_io_cfg.o

dev_io_INCLUDE_DIRS = $(dev_io_DIR) \
    $(dev_io_utils_INCLUDE_DIRS) \
    $(LIBCLIENT_DIR)

dev_io_cfg_INCLUDE_DIRS = $(dev_io_DIR) \
    $(dev_io_utils_INCLUDE_DIRS) \
    $(LIBCLIENT_DIR)

dev_io_OUT = dev_io dev_io_cfg

dev_io_all_OUT = $(dev_io_OUT)

