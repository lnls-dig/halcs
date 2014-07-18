sm_io_swap_DIR = hal/sm_io/modules/swap

sm_io_swap_OBJS = $(sm_io_swap_DIR)/sm_io_swap_core.o \
		 $(sm_io_swap_DIR)/sm_io_swap_exp.o \
		 $(sm_io_swap_DIR)/sm_io_swap_defaults.o

sm_io_swap_INCLUDE_DIRS = $(sm_io_swap_DIR)
