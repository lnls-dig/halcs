revision_DIR = $(SRC_DIR)/revision

revision_OBJS = $(revision_DIR)/revision.o

revision_INCLUDE_DIRS = $(addprefix -I, $(revision_DIR))

