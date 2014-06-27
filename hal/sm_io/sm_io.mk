include hal/sm_io/modules/modules.mk hal/sm_io/rw_param/rw_param.mk

sm_io_DIR = hal/sm_io

sm_io_OBJS = $(sm_io_DIR)/sm_io.o \
	     $(sm_io_DIR)/sm_io_bootstrap.o \
	     $(sm_io_DIR)/sm_io_err.o \
	     $(sm_io_DIR)/sm_io_thsafe_codes.o \
	     $(sm_io_modules_OBJS) \
	     $(sm_io_rw_param_OBJS)

sm_io_INCLUDE_DIRS = $(sm_io_DIR) \
		     $(sm_io_modules_INCLUDE_DIRS) \
		     $(sm_io_rw_param_INCLUDE_DIRS)

