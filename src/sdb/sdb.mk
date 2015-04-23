sdb_DIR = $(SRC_DIR)/sdb

# Here we call <output_name>_core_OBJS as we need to add
# more objects to this target. This is done in the hal.mk
# makefile
#sdb_core_OBJS = $(sdb_DIR)/sdb.o
sdb_ALL_DIRS = $(sdb_DIR)

# Add -I prefix to all include directories
sdb_INCLUDE_DIRS = $(addprefix -I, $(sdb_ALL_DIRS))
