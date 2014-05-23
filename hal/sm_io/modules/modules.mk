include hal/sm_io/modules/fmc130m_4ch/fmc130m_4ch.mk \
	hal/sm_io/modules/acq/acq.mk

sm_io_modules_DIR = hal/sm_io/modules

sm_io_modules_OBJS = $(sm_io_modules_DIR)/sm_io_mod_dispatch.o \
		     $(sm_io_fmc130m_4ch_OBJS) \
		     $(sm_io_acq_OBJS)

sm_io_modules_INCLUDE_DIRS = $(sm_io_modules_DIR) \
			     $(sm_io_fmc130m_4ch_DIR)\
			     $(sm_io_acq_DIR)
