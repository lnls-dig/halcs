sm_io_afc_diag_DIR = $(sm_io_modules_DIR)/afc_diag

# Only compile this for supported AFCv3 boards
ifeq ($(BOARD),$(filter $(BOARD),$(SUPPORTED_AFCV3_BOARDS)))
sm_io_afc_diag_OBJS = $(sm_io_afc_diag_DIR)/sm_io_afc_diag_core.o \
		 $(sm_io_afc_diag_DIR)/sm_io_afc_diag_exp.o \
		 $(sm_io_afc_diag_DIR)/sm_io_afc_diag_defaults.o
endif
