include hal/sm_io/modules/fmc130m_4ch/fmc130m_4ch.mk \
		hal/sm_io/modules/acq/acq.mk \
		hal/sm_io/modules/dsp/dsp.mk \
		hal/sm_io/modules/swap/swap.mk \
		hal/sm_io/modules/rffe/rffe.mk

sm_io_modules_DIR = hal/sm_io/modules

sm_io_modules_OBJS = $(sm_io_modules_DIR)/sm_io_mod_dispatch.o \
		     $(sm_io_modules_DIR)/sm_io_codes.o \
		     $(sm_io_fmc130m_4ch_OBJS) \
		     $(sm_io_acq_OBJS) \
		     $(sm_io_dsp_OBJS) \
		     $(sm_io_swap_OBJS) \
		     $(sm_io_rffe_OBJS)

sm_io_modules_INCLUDE_DIRS = $(sm_io_modules_DIR) \
			     $(sm_io_fmc130m_4ch_DIR) \
			     $(sm_io_acq_DIR) \
			     $(sm_io_dsp_DIR) \
			     $(sm_io_swap_DIR) \
			     $(sm_io_rffe_DIR)
