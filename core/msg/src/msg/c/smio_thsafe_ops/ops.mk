smio_thsafe_ops_DIR = $(SRC_DIR)/core/msg/src/msg/c/smio_thsafe_ops

smio_thsafe_ops_OBJS = $(smio_thsafe_ops_DIR)/smio_thsafe_zmq_client.o \
		       $(smio_thsafe_ops_DIR)/smio_thsafe_zmq_server.o \
		       $(smio_thsafe_ops_DIR)/thsafe_msg_zmq.o
