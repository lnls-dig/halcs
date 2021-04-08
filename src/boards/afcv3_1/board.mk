board_DIR = $(SRC_DIR)/boards/afcv3_1

board_OBJS = $(board_DIR)/acq_chan.o \
			 $(board_DIR)/chips_addr.o \
			 $(board_DIR)/device_mapping.o \
			 $(board_DIR)/ddr3_map.o
