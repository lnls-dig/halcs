sm_io_afc_diag_DIR = $(SRC_DIR)/sm_io/modules/afc_diag

sm_io_afc_diag_OBJS = $(sm_io_afc_diag_DIR)/sm_io_afc_diag_exports.o

ifeq ($(BOARD),afcv3)
sm_io_afc_diag_OBJS += $(sm_io_afc_diag_DIR)/sm_io_afc_diag_core.o \
		 $(sm_io_afc_diag_DIR)/sm_io_afc_diag_exp.o \
		 $(sm_io_afc_diag_DIR)/sm_io_afc_diag_defaults.o
endif
