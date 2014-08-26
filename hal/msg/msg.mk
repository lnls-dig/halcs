include hal/msg/exp_ops/ops.mk
include hal/msg/smio_thsafe_ops/ops.mk

msg_DIR = hal/msg

msg_OBJS = $(msg_DIR)/msg.o \
	   $(msg_DIR)/msg_err.o \
	   $(exp_ops_OBJS) \
	   $(smio_thsafe_ops_OBJS)

msg_INCLUDE_DIRS = $(msg_DIR) \
		   $(exp_ops_INCLUDE_DIRS) \
		   $(smio_thsafe_ops_INCLUDE_DIRS)

# FIXME: For use in hal.mk with dev_mngr_OBJS
thsafe_msg_zmq_OBJS = $(smio_thsafe_ops_DIR)/thsafe_msg_zmq.o \
		      $(msg_DIR)/msg_err.o
