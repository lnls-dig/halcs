sm_io_rffe_DIR = $(SRC_DIR)/sm_io/modules/rffe

sm_io_rffe_OBJS = $(sm_io_rffe_DIR)/sm_io_rffe_core.o \
		 $(sm_io_rffe_DIR)/sm_io_rffe_exp.o \
		 $(sm_io_rffe_DIR)/sm_io_rffe_exports.o \
		 $(sm_io_rffe_DIR)/sm_io_rffe_defaults.o

sm_io_rffe_INCLUDE_DIRS = $(sm_io_rffe_DIR)
