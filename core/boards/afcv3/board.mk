board_DIR = $(SRC_DIR)/core/boards/afcv3/src/afcv3/c

board_OBJS = $(board_DIR)/epics_mapping.o \
			 $(board_DIR)/ddr3_map.o \
			 $(board_DIR)/chips_addr.o

