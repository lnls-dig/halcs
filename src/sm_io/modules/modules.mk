include $(SRC_DIR)/sm_io/modules/fmc130m_4ch/fmc130m_4ch.mk \
		$(SRC_DIR)/sm_io/modules/acq/acq.mk \
		$(SRC_DIR)/sm_io/modules/dsp/dsp.mk \
		$(SRC_DIR)/sm_io/modules/swap/swap.mk \
		$(SRC_DIR)/sm_io/modules/rffe/rffe.mk \
		$(SRC_DIR)/sm_io/modules/afc_diag/afc_diag.mk

sm_io_modules_DIR = $(SRC_DIR)/sm_io/modules

sm_io_modules_OBJS = $(sm_io_modules_DIR)/sm_io_mod_dispatch.o \
		     $(sm_io_modules_DIR)/sm_io_codes.o \
		     $(sm_io_fmc130m_4ch_OBJS) \
		     $(sm_io_acq_OBJS) \
		     $(sm_io_dsp_OBJS) \
		     $(sm_io_swap_OBJS) \
		     $(sm_io_rffe_OBJS) \
		     $(sm_io_afc_diag_OBJS)
