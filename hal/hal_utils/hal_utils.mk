hal_utils_DIR = hal/hal_utils

# We depend on "msg.c" fucntions to determine which message
# we are dealing with
hal_utils_OBJS = $(hal_utils_DIR)/hal_utils.o \
		 $(hal_utils_DIR)/hal_utils_err.o \
		 $(hal_utils_DIR)/dispatch_table.o \
		 $(msg_DIR)/msg.o 

hal_utils_INCLUDE_DIRS = $(hal_utils_DIR)

