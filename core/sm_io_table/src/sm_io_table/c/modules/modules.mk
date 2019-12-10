sm_io_table_modules_DIR = $(sm_io_table_DIR)/modules

include $(sm_io_table_modules_DIR)/fmc130m_4ch/fmc130m_4ch.mk \
		$(sm_io_table_modules_DIR)/fmc250m_4ch/fmc250m_4ch.mk \
		$(sm_io_table_modules_DIR)/fmc_adc_common/fmc_adc_common.mk \
		$(sm_io_table_modules_DIR)/fmc_active_clk/fmc_active_clk.mk \
		$(sm_io_table_modules_DIR)/acq/acq.mk \
		$(sm_io_table_modules_DIR)/acq_pm/acq_pm.mk \
		$(sm_io_table_modules_DIR)/dsp/dsp.mk \
		$(sm_io_table_modules_DIR)/swap/swap.mk \
		$(sm_io_table_modules_DIR)/rffe/rffe.mk \
		$(sm_io_table_modules_DIR)/afc_diag/afc_diag.mk \
		$(sm_io_table_modules_DIR)/trigger_iface/trigger_iface.mk \
		$(sm_io_table_modules_DIR)/trigger_mux/trigger_mux.mk \
		$(sm_io_table_modules_DIR)/afc_timing/afc_timing.mk \
		$(sm_io_table_modules_DIR)/init/init.mk \
		$(sm_io_table_modules_DIR)/afc_mgmt/afc_mgmt.mk \
		$(sm_io_table_modules_DIR)/tim_rcv/tim_rcv.mk \
		$(sm_io_table_modules_DIR)/rffe_scpi/rffe_scpi.mk

sm_io_table_modules_OBJS = $(sm_io_table_modules_DIR)/sm_io_codes.o \
		     $(sm_io_table_fmc130m_4ch_OBJS) \
		     $(sm_io_table_fmc250m_4ch_OBJS) \
		     $(sm_io_table_fmc_adc_common_OBJS) \
		     $(sm_io_table_fmc_active_clk_OBJS) \
		     $(sm_io_table_acq_OBJS) \
		     $(sm_io_table_acq_pm_OBJS) \
		     $(sm_io_table_dsp_OBJS) \
		     $(sm_io_table_swap_OBJS) \
		     $(sm_io_table_rffe_OBJS) \
		     $(sm_io_table_afc_diag_OBJS) \
		     $(sm_io_table_trigger_iface_OBJS) \
		     $(sm_io_table_trigger_mux_OBJS) \
		     $(sm_io_table_afc_timing_OBJS) \
		     $(sm_io_table_init_OBJS) \
		     $(sm_io_table_afc_mgmt_OBJS) \
		     $(sm_io_table_tim_rcv_OBJS) \
		     $(sm_io_table_rffe_scpi_OBJS)
