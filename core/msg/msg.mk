msg_DIR = $(SRC_DIR)/core/msg/src/msg/c

include $(msg_DIR)/exp_ops/ops.mk
include $(msg_DIR)/smio_thsafe_ops/ops.mk

msg_OBJS = $(msg_DIR)/msg.o \
	   $(msg_DIR)/msg_err.o \
	   $(exp_ops_OBJS) \
	   $(smio_thsafe_ops_OBJS)
