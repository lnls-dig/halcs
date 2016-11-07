sm_io_fmc_adc_common_DIR = $(sm_io_modules_DIR)/fmc_adc_common

sm_io_fmc_adc_common_OBJS = $(sm_io_fmc_adc_common_DIR)/sm_io_fmc_adc_common_core.o \
				$(sm_io_fmc_adc_common_DIR)/sm_io_fmc_adc_common_exp.o \
				$(sm_io_fmc_adc_common_DIR)/sm_io_fmc_adc_common_exports.o \
				$(sm_io_fmc_adc_common_DIR)/sm_io_fmc_adc_common_defaults.o
