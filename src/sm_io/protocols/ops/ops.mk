sm_io_protocols_ops_DIR = $(sm_io_protocols_DIR)/ops

sm_io_protocols_ops_OBJS = $(sm_io_protocols_ops_DIR)/sm_pr_spi.o \
		 $(sm_io_protocols_ops_DIR)/sm_pr_i2c.o \
		 $(sm_io_protocols_ops_DIR)/sm_pr_bsmp.o \
		 $(sm_io_protocols_ops_DIR)/sm_pr_scpi.o \
#		 $(sm_io_protocols_ops_DIR)/sm_pr_bypass.o \
#		 $(sm_io_protocols_ops_DIR)/sm_pr_1w.o \
#		 $(sm_io_protocols_ops_DIR)/sm_pr_gpio.o
