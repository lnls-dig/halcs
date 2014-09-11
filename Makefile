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

# Select board in which we will work. Options are: ml605 or afcv3
BOARD ?= ml605

INSTALL_DIR ?= /usr/local
export INSTALL_DIR

INIT_SCRIPTS = init.sh shutdown.sh

# Kernel stuff (pcie driver and library) relative
# directory
KERNEL_DIR = kernel
KERNEL_VER = $(shell uname -r)
DRIVER_OBJ = /lib/modules/$(KERNEL_VER)/extra/pciDriver.ko

# Client library
LIBCLIENT_DIR=libclient

# General C flags
CFLAGS = -std=gnu99 -O2

ifeq ($(BOARD),ml605)
CFLAGS += -D__BOARD_ML605__
endif

ifeq ($(BOARD),afcv3)
CFLAGS += -D__BOARD_AFCV3__
endif

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
	       -I$(KERNEL_DIR)/include/pcie \
	       -I/usr/local/include

# Merge all flags.
CFLAGS += $(CFLAGS_PLATFORM) $(CFLAGS_DEBUG)

LDFLAGS = $(LDFLAGS_PLATFORM)

# Output modules
OUT = $(hal_OUT)

.SECONDEXPANSION:

# Save a git repository description
REVISION = $(shell git describe --dirty --always)
REVISION_NAME = revision
OBJ_REVISION = $(addsuffix .o, $(REVISION_NAME))

OBJS_all =  $(hal_OBJS) $(OBJ_REVISION)

.PHONY: all kernel clean mrproper install uninstall tests examples \
	kernel_install kernel_uninstall kernel_check \
	libclient libclient_install libclient_uninstall libclient_mrproper \
	hal_install hal_uninstall hal_clean hal_mrproper \
	tests tests_clean tests_mrproper \
	examples_clean examples_mrproper

# Avoid deletion of intermediate files, such as objects
.SECONDARY: $(OBJS_all)

# Makefile rules
all: kernel libclient $(OUT)

# Output Rule
$(OUT): $$($$@_OBJS) $(REVISION_NAME).o
	$(CC) $(LFLAGS) $(CFLAGS) $(INCLUDE_DIRS) -o $@ $^ $($@_STATIC_LIBS) $(LDFLAGS) $(LIBS) $($@_LIBS)

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

kernel: kernel_check
	$(MAKE) -C $(KERNEL_DIR) all

#Verify if the driver is in place
kernel_check:
ifeq ($(wildcard $(DRIVER_OBJ)),)
	@echo "PCI driver not found!";
	@echo "Compilation will continue, but you must install";
	@echo "and load the driver prior to initializing the software";
	@sleep 2;
endif

kernel_install:
	$(MAKE) -C $(KERNEL_DIR) install

kernel_uninstall:
	$(MAKE) -C $(KERNEL_DIR) uninstall

kernel_clean:
	$(MAKE) -C $(KERNEL_DIR) clean

libclient:
	$(MAKE) -C $(LIBCLIENT_DIR) all

libclient_install:
	$(MAKE) -C $(LIBCLIENT_DIR) install

libclient_uninstall:
	$(MAKE) -C $(LIBCLIENT_DIR) uninstall

libclient_clean:
	$(MAKE) -C $(LIBCLIENT_DIR) clean

libclient_mrproper:
	$(MAKE) -C $(LIBCLIENT_DIR) mrproper

hal_install:
	$(foreach hal_bin,$(OUT),install -m 755 $(hal_bin) $(INSTALL_DIR)/bin $(CMDSEP))
	$(foreach hal_script,$(INIT_SCRIPTS),install -m 755 $(hal_script) $(INSTALL_DIR)/etc $(CMDSEP))

hal_uninstall:
	$(foreach hal_bin,$(OUT),rm -f $(INSTALL_DIR)/bin/$(hal_bin) $(CMDSEP))
	$(foreach hal_script,$(INIT_SCRIPTS),rm -f $(INSTALL_DIR)/etc/$(hal_script) $(CMDSEP))

hal_clean:
	rm -f $(OBJS_all) $(OBJS_all:.o=.d)

hal_mrproper:
	rm -f $(OUT)

tests:
	$(MAKE) -C tests all

tests_clean:
	$(MAKE) -C tests clean

tests_mrproper:
	$(MAKE) -C tests mrproper

examples:
	$(MAKE) -C examples all

examples_clean:
	$(MAKE) -C examples clean

examples_mrproper:
	$(MAKE) -C examples mrproper

install: hal_install kernel_install libclient_install

uninstall: hal_uninstall kernel_uninstall libclient_uninstall

clean: hal_clean kernel_clean libclient_clean examples_clean tests_clean

mrproper: clean hal_mrproper libclient_mrproper examples_mrproper libclient_mrproper

