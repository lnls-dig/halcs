sm_io_trigger_iface_DIR = $(SRC_DIR)/sm_io/modules/trigger_iface

sm_io_trigger_iface_OBJS = $(sm_io_trigger_iface_DIR)/sm_io_trigger_iface_core.o \
				$(sm_io_trigger_iface_DIR)/sm_io_trigger_iface_exp.o \
				$(sm_io_trigger_iface_DIR)/sm_io_trigger_iface_exports.o \
				$(sm_io_trigger_iface_DIR)/sm_io_trigger_iface_defaults.o
