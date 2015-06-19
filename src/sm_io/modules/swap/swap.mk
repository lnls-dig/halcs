sm_io_swap_DIR = $(SRC_DIR)/sm_io/modules/swap

sm_io_swap_OBJS = $(sm_io_swap_DIR)/sm_io_swap_core.o \
		 $(sm_io_swap_DIR)/sm_io_swap_exp.o \
		 $(sm_io_swap_DIR)/sm_io_swap_exports.o \
		 $(sm_io_swap_DIR)/sm_io_swap_defaults.o
