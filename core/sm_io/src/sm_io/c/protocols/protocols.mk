sm_io_protocols_DIR = $(sm_io_DIR)/protocols

include $(sm_io_protocols_DIR)/ops/ops.mk

# Here we call <output_name>_core_OBJS as we need to add
# more objects to this target. This is done in the hal.mk
# makefile
sm_io_protocols_OBJS =  $(sm_io_protocols_DIR)/sm_pr.o \
			$(sm_io_protocols_DIR)/sm_pr_err.o \
			$(sm_io_protocols_ops_OBJS)
