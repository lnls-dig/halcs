sm_io_fmc_active_clk_DIR = $(SRC_DIR)/sm_io/modules/fmc_active_clk

sm_io_fmc_active_clk_OBJS = $(sm_io_fmc_active_clk_DIR)/sm_io_fmc_active_clk_core.o \
				$(sm_io_fmc_active_clk_DIR)/sm_io_fmc_active_clk_exp.o \
				$(sm_io_fmc_active_clk_DIR)/sm_io_fmc_active_clk_exports.o \
				$(sm_io_fmc_active_clk_DIR)/sm_io_fmc_active_clk_defaults.o
