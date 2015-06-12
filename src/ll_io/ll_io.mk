include $(SRC_DIR)/ll_io/ops/ops.mk

ll_io_DIR = $(SRC_DIR)/ll_io

ll_io_utils_OBJS = $(ll_io_DIR)/ll_io_utils.o

# Here we call <output_name>_core_OBJS as we need to add
# more objects to this target. This is done in the hal.mk
# makefile
ll_io_OBJS = $(ll_io_DIR)/ll_io.o \
	     $(ll_io_DIR)/ll_io_endpoint.o \
	     $(ll_io_DIR)/ll_io_dev_info.o \
	     $(ll_io_DIR)/ll_io_err.o \
	     $(ll_io_utils_OBJS) \
	     $(ll_io_ops_OBJS)
