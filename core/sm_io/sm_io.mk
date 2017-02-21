sm_io_DIR = core/sm_io/src/sm_io/c

include $(sm_io_DIR)/modules/modules.mk
include $(sm_io_DIR)/rw_param/rw_param.mk
include $(sm_io_DIR)/protocols/protocols.mk
include $(sm_io_DIR)/chips/chips.mk

sm_io_OBJS = $(sm_io_DIR)/sm_io.o \
	     $(sm_io_DIR)/sm_io_bootstrap.o \
	     $(sm_io_DIR)/sm_io_err.o \
	     $(sm_io_modules_OBJS) \
	     $(sm_io_rw_param_OBJS) \
	     $(sm_io_protocols_OBJS) \
	     $(sm_io_chips_OBJS)
