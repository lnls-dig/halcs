# Set your cross compile prefix with CROSS_COMPILE variable
CROSS_COMPILE ?=

CMDSEP = ;

CC =		$(CROSS_COMPILE)gcc
AR =		$(CROSS_COMPILE)ar
LD =		$(CROSS_COMPILE)ld
OBJDUMP =	$(CROSS_COMPILE)objdump
OBJCOPY =	$(CROSS_COMPILE)objcopy
SIZE =		$(CROSS_COMPILE)size
MAKE =		make

# Select board in which we will work. Options are: ml605, afc
BOARD = ml605

INSTALL_DIR ?= /usr/lib
export INSTALL_DIR

# Kernel stuff (pcie driver and library) relative
# directory
KERNEL_DIR = kernel

# General C flags
CFLAGS = -std=gnu99 -O2 -DWR_SHIFT=2

LOCAL_MSG_DBG ?= n
DBE_DBG ?= n
CFLAGS_DEBUG =

ifeq ($(LOCAL_MSG_DBG),y)
CFLAGS_DEBUG += -DLOCAL_MSG_DBG=1
endif

ifeq ($(DBE_DBG),y)
CFLAGS_DEBUG += -DDBE_DBG=1
endif

# Debug flags -D<flasg_name>=<value>
CFLAGS_DEBUG += -g

# Specific platform Flags
CFLAGS_PLATFORM = -Wall -Wextra -Werror
LDFLAGS_PLATFORM =

# Libraries
LIBS = -lzmq -lczmq -lmdp -lpcidriver
# General library flags -L<libdir>
LFLAGS =

# Specific platform objects
OBJS_PLATFORM =

# Include other Makefiles as needed here
#include services/services.mk
#include clients/clients.mk
include hal/hal.mk

# Include directories
INCLUDE_DIRS = $(hal_INCLUDE_DIRS) \
	       -I$(KERNEL_DIR)/include/pcie

# Merge all flags. Optimize for size (-Os)
CFLAGS += $(CFLAGS_PLATFORM) $(CFLAGS_DEBUG)
#-Os

LDFLAGS = $(LDFLAGS_PLATFORM)
#-ffunction-sections -fdata-sections -Wl,--gc-sections

# Output modules
OUT = $(hal_OUT)

.SECONDEXPANSION:

# Save a git repository description
REVISION = $(shell git describe --dirty --always)
REVISION_NAME = revision
OBJ_REVISION = $(addsuffix .o, $(REVISION_NAME))

OBJS_all =  $(hal_OBJS) $(OBJ_REVISION)

.PHONY: all kernel_install kernel_check \
	clean mrproper install uninstall \
	tests examples

# Avoid deletion of intermediate files, such as objects
.SECONDARY: $(OBJS_all)

# Makefile rules
all: kernel_install $(OUT)

# Output Rule
$(OUT): $$($$@_OBJS) $(REVISION_NAME).o
	$(CC) $(LFLAGS) $(CFLAGS) $(INCLUDE_DIRS) -o $@ $^ $(LDFLAGS) $(LIBS)

$(REVISION_NAME).o: $(REVISION_NAME).c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -DGIT_REVISION=\"$(REVISION)\" -c $<

# Pull in dependency info for *existing* .o files and don't complain if the
# corresponding .d file is not found
-include $(OBJS_all:.o=.d)

# Autodependencies generatation by Scott McPeak, November 2001,
# from article "Autodependencies with GNU make"
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $*.c -o $@

# create the dependency files "target: pre-requisites"
	${CC} -MM $(CFLAGS) $(INCLUDE_DIRS) $*.c > $*.d

# Workaround to make objects in different folders have
# the correct target path. e.g., "dir/bar.o: dir/bar.c dir/foo.h"
# instead of "bar.o: dir/bar.c dir/foo.h"
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d

# All prereqs listed will also become command-less,
# prereq-less targets. In this way, the prereq file will be
# treated as changed and the target will be rebuilt
#   sed:    strip the target (everything before colon)
#   sed:    remove any continuation backslashes
#   fmt -1: list words one per line
#   sed:    strip leading spaces
#   sed:    add trailing colons
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
		sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

kernel_check:
	$(MAKE) -C $(KERNEL_DIR) all

kernel_install: kernel_check
	$(MAKE) -C $(KERNEL_DIR) install

tests:
	$(MAKE) -C $@ all

examples:
	$(MAKE) -C $@ all

#install:
#	@install -m 755 $(TARGET_SHARED_VER) $(INSTALL_DIR)
#	$(foreach lib,$(TARGET_SHARED),ln -sf $(lib).$(LIB_VER) $(INSTALL_DIR)/$(lib) $(CMDSEP))
#
#uninstall:
#	$(foreach lib,$(TARGET_SHARED),rm -f $(INSTALL_DIR)/$(lib).$(LIB_VER) $(CMDSEP))
#	$(foreach lib,$(TARGET_SHARED),rm -f $(INSTALL_DIR)/$(lib) $(CMDSEP))

clean:
	rm -f $(OBJS_all) $(OBJS_all:.o=.d)
	$(MAKE) -C tests clean
	$(MAKE) -C examples clean
	$(MAKE) -C libclient clean
	$(MAKE) -C $(KERNEL_DIR) clean

mrproper: clean
	rm -f $(OUT)
	$(MAKE) -C tests mrproper
	$(MAKE) -C examples mrproper
	$(MAKE) -C libclient mrproper
