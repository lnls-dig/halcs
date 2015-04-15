debug_DIR = $(SRC_DIR)/hal/debug

debug_OBJS = $(debug_DIR)/debug_print.o \
	     $(debug_DIR)/local_print.o \
	     $(debug_DIR)/debug_subsys.o
debug_INCLUDE_DIRS = $(debug_DIR)
