sm_io_table_DIR = $(SRC_DIR)/sm_io_table

include $(sm_io_table_DIR)/modules/modules.mk

sm_io_table_OBJS = $(sm_io_table_DIR)/sm_io_codes.o \
                   $(sm_io_table_modules_OBJS)
