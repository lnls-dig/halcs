sm_io_dsp_DIR = $(SRC_DIR)/hal/sm_io/modules/dsp

sm_io_dsp_OBJS = $(sm_io_dsp_DIR)/sm_io_dsp_core.o \
		 $(sm_io_dsp_DIR)/sm_io_dsp_exp.o \
		 $(sm_io_dsp_DIR)/sm_io_dsp_exports.o \
		 $(sm_io_dsp_DIR)/sm_io_dsp_defaults.o

sm_io_dsp_INCLUDE_DIRS = $(sm_io_dsp_DIR)
