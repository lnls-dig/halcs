revision_DIR = $(SRC_DIR)/revision

revision_OBJS = $(revision_DIR)/revision.o

revision_ALL_DIRS = $(revision_DIR)

# Add -I prefix to all include directories
revision_INCLUDE_DIRS = $(addprefix -I, $(revision_ALL_DIRS))
