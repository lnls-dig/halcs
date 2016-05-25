sm_io_trigger_mux_DIR = $(SRC_DIR)/sm_io/modules/trigger_mux

sm_io_trigger_mux_OBJS = $(sm_io_trigger_mux_DIR)/sm_io_trigger_mux_core.o \
				$(sm_io_trigger_mux_DIR)/sm_io_trigger_mux_exp.o \
				$(sm_io_trigger_mux_DIR)/sm_io_trigger_mux_exports.o \
				$(sm_io_trigger_mux_DIR)/sm_io_trigger_mux_defaults.o
