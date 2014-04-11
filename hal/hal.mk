# debug.mk must be declared before dev_mngr.mk, as they are variables
# exported by the former used by the latter
include hal/debug/debug.mk
include hal/hal_utils/hal_utils.mk
include hal/ll_io/ll_io.mk
include hal/sm_io/sm_io.mk
include hal/sdb/sdb.mk
include hal/dev_mngr/dev_mngr.mk
include hal/dev_io/dev_io.mk
include hal/msg/msg.mk

std_hal_INCLUDE_DIRS = hal/include

hal_OUT += $(dev_mngr_OUT) $(dev_io_OUT)

# For each target in hal_OUT we add the necessary objects
dev_mngr_OBJS = $(dev_mngr_core_OBJS) $(debug_OBJS)
dev_io_OBJS = $(dev_io_core_OBJS) $(ll_io_OBJS) $(sm_io_OBJS) \
	      $(msg_OBJS) $(debug_OBJS) $(hal_utils_OBJS)

# Merge all hal objects together
hal_OBJS = $(debug_OBJS) \
	   $(hal_utils_OBJS) \
	   $(ll_io_OBJS) \
	   $(sm_io_OBJS) \
	   $(msg_OBJS) \
	   $(dev_mngr_core_OBJS) \
	   $(dev_io_core_OBJS)

# Merge all include directories together
hal_all_INCLUDE_DIRS += $(std_hal_INCLUDE_DIRS) \
	$(debug_INCLUDE_DIRS) \
	$(hal_utils_INCLUDE_DIRS) \
	$(sdb_INCLUDE_DIRS) \
	$(ll_io_INCLUDE_DIRS) \
	$(sm_io_INCLUDE_DIRS) \
	$(msg_INCLUDE_DIRS) \
	$(dev_mngr_INCLUDE_DIRS) \
	$(dev_io_INCLUDE_DIRS)

# Change the include dirs into flags
hal_INCLUDE_DIRS = $(addprefix -I, $(hal_all_INCLUDE_DIRS))
