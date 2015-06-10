include $(SRC_DIR)/msg/exp_ops/ops.mk
include $(SRC_DIR)/msg/smio_thsafe_ops/ops.mk

msg_DIR = $(SRC_DIR)/msg

msg_OBJS = $(msg_DIR)/msg.o \
	   $(msg_DIR)/msg_err.o \
	   $(exp_ops_OBJS) \
	   $(smio_thsafe_ops_OBJS)
