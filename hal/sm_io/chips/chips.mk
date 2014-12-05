sm_io_chips_DIR = hal/sm_io/chips

# Here we call <output_name>_core_OBJS as we need to add
# more objects to this target. This is done in the hal.mk
# makefile
sm_io_chips_OBJS =  $(sm_io_chips_DIR)/sm_ch_ad9510.o \
            $(sm_io_chips_DIR)/sm_ch_24aa64.o \
            $(sm_io_chips_DIR)/sm_ch_pca9547.o \
            $(sm_io_chips_DIR)/sm_ch_si57x.o \
			$(sm_io_chips_DIR)/sm_ch_err.o

sm_io_chips_INCLUDE_DIRS = $(sm_io_chips_DIR)
