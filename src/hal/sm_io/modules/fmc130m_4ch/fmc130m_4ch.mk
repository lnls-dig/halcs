sm_io_fmc130m_4ch_DIR = $(SRC_DIR)/hal/sm_io/modules/fmc130m_4ch

sm_io_fmc130m_4ch_OBJS = $(sm_io_fmc130m_4ch_DIR)/sm_io_fmc130m_4ch_core.o \
				$(sm_io_fmc130m_4ch_DIR)/sm_io_fmc130m_4ch_exp.o \
				$(sm_io_fmc130m_4ch_DIR)/sm_io_fmc130m_4ch_structs.o \
				$(sm_io_fmc130m_4ch_DIR)/sm_io_fmc130m_4ch_exports.o \
				$(sm_io_fmc130m_4ch_DIR)/sm_io_fmc130m_4ch_defaults.o

sm_io_fmc130m_4ch_INCLUDE_DIRS = $(sm_io_fmc130m_4ch_DIR)