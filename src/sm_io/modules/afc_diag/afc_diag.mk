sm_io_afc_diag_DIR = $(sm_io_modules_DIR)/afc_diag

# Only compile this for supported AFCv3 boards
sm_io_afc_diag_OBJS = $(sm_io_afc_diag_DIR)/sm_io_afc_diag_core.o \
		 $(sm_io_afc_diag_DIR)/sm_io_afc_diag_exp.o \
		 $(sm_io_afc_diag_DIR)/sm_io_afc_diag_defaults.o
