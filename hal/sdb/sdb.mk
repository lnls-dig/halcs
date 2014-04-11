sdb_DIR = hal/sdb

# Here we call <output_name>_core_OBJS as we need to add
# more objects to this target. This is done in the hal.mk
# makefile
#sdb_core_OBJS = $(sdb_DIR)/sdb.o
sdb_INCLUDE_DIRS = $(sdb_DIR)

sdb_OUT = sdb
