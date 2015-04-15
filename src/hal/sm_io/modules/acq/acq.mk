sm_io_acq_DIR = $(SRC_DIR)/hal/sm_io/modules/acq

sm_io_acq_OBJS = $(sm_io_acq_DIR)/sm_io_acq_core.o \
		 $(sm_io_acq_DIR)/sm_io_acq_exp.o \
		 $(sm_io_acq_DIR)/sm_io_acq_exports.o \
		 $(sm_io_acq_DIR)/ddr3_map.o

sm_io_acq_INCLUDE_DIRS = $(sm_io_acq_DIR)
