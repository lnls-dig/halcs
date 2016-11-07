dev_io_DIR = $(SRC_DIR)/core/dev_io/src/dev_io/c

dev_io_core_utils_OBJS = $(dev_io_DIR)/dev_io_utils.o

# Here we call <output_name>_core_OBJS as we need to add
# more objects to this target. This is done in the hal.mk
# makefile
dev_io_core_OBJS = $(dev_io_DIR)/dev_io_core.o \
		   $(dev_io_DIR)/dev_io_err.o \
		   $(dev_io_core_utils_OBJS)

