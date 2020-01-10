sm_io_chips_DIR = $(sm_io_DIR)/chips

# Here we call <output_name>_core_OBJS as we need to add
# more objects to this target. This is done in the hal.mk
# makefile
sm_io_chips_OBJS =  $(sm_io_chips_DIR)/sm_ch_ad9510.o \
            $(sm_io_chips_DIR)/sm_ch_isla216p.o \
            $(sm_io_chips_DIR)/sm_ch_24aa64.o \
            $(sm_io_chips_DIR)/sm_ch_pca9547.o \
            $(sm_io_chips_DIR)/sm_ch_si57x.o \
            $(sm_io_chips_DIR)/sm_ch_rffe.o \
            $(sm_io_chips_DIR)/sm_ch_rffe_scpi.o \
			$(sm_io_chips_DIR)/sm_ch_err.o
