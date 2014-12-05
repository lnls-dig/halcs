smio_thsafe_ops_DIR = hal/msg/smio_thsafe_ops

smio_thsafe_ops_OBJS = $(smio_thsafe_ops_DIR)/smio_thsafe_zmq_client.o \
		       $(smio_thsafe_ops_DIR)/smio_thsafe_zmq_server.o \
		       $(smio_thsafe_ops_DIR)/thsafe_msg_zmq.o

smio_thsafe_ops_INCLUDE_DIRS = $(smio_thsafe_ops_DIR)
