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
#Select if we want to compile code with all messages outputs. Options are: y(es) or n(o)
LOCAL_MSG_DBG ?= n
#Select if we want to compile with debug mode on. Options are: y(es) or n(o)
DBE_DBG ?= y
# Select the FMC ADC board type. Options are: passive or active
FMC130M_4CH_TYPE ?= passive
# Select if we should program FMC EEPROM with some code or not. Option are:
# active, passive or nothing (dont' program EEPROM)
FMC130M_4CH_EEPROM_PROGRAM ?=
# Selects if we want to compile DEV_MNGR. Options are: y(es) or n(o)
WITH_DEV_MNGR ?= y
# Selects the AFE RFFE version. Options are: 2
AFE_RFFE_TYPE ?= 2
# Selects the install location of the config file
CFG_DIR ?= /etc/bpm_sw
export CFG_DIR

INSTALL_DIR ?= /usr/local
export INSTALL_DIR

# Config filename
CFG_FILENAME = bpm_sw.cfg

INIT_SCRIPTS = init.sh shutdown.sh

# Subdmoules and third-party codes
FOREIGN_DIR = foreign

# Our submodules and third-party codes
LIBMDP_DIR = $(FOREIGN_DIR)/libmdp/libmdp
LIBBSMP_DIR = $(FOREIGN_DIR)/libbsmp
PCIE_DRIVER_DIR = $(FOREIGN_DIR)/pcie-driver

# PCIe driver stuff (pcie driver and library) relative
# directory
PCIE_DRIVER_VER = $(shell uname -r)
DRIVER_OBJ = /lib/modules/$(PCIE_DRIVER_VER)/extra/pciDriver.ko

# Client library
LIBCLIENT_DIR=libclient

# General C flags
CFLAGS = -std=gnu99 -O2

# Board selection
ifeq ($(BOARD),ml605)
CFLAGS += -D__BOARD_ML605__ -D__WR_SHIFT_FIX__=2
endif

ifeq ($(BOARD),afcv3)
CFLAGS += -D__BOARD_AFCV3__ -D__WR_SHIFT_FIX__=0
endif

# Program FMC130M_4CH EEPROM
ifeq ($(FMC130M_4CH_EEPROM_PROGRAM),active)
CFLAGS += -D__FMC130M_4CH_EEPROM_PROGRAM__=1
endif

ifeq ($(FMC130M_4CH_EEPROM_PROGRAM),passive)
CFLAGS += -D__FMC130M_4CH_EEPROM_PROGRAM__=2
endif

# Compile DEV MNGR or not
ifeq ($(WITH_DEV_MNGR),y)
CFLAGS += -D__WITH_DEV_MNGR__
endif

ifeq ($(AFE_RFFE_TYPE),1)
CFLAGS += -D__AFE_RFFE_V1__
endif

ifeq ($(AFE_RFFE_TYPE),2)
CFLAGS += -D__AFE_RFFE_V2__
endif

ifneq ($(CFG_DIR),)
CFLAGS += -D__CFG_DIR__=$(CFG_DIR)
endif

ifneq ($(CFG_FILENAME),)
CFLAGS += -D__CFG_FILENAME__=$(CFG_FILENAME)
endif

# Debug conditional flags
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
LIBS = -lm -lzmq -lczmq -lmdp -lpcidriver
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
	       -I$(PCIE_DRIVER_DIR)/include/pcie \
	       -I/usr/local/include

# Merge all flags.
CFLAGS += $(CFLAGS_PLATFORM) $(CFLAGS_DEBUG)

LDFLAGS = $(LDFLAGS_PLATFORM)

# Output modules
OUT = $(hal_OUT)

# All possible output modules
ALL_OUT = $(hal_all_OUT)

.SECONDEXPANSION:

# Save a git repository description
REVISION = $(shell git describe --dirty --always)
REVISION_NAME = revision
OBJ_REVISION = $(addsuffix .o, $(REVISION_NAME))

OBJS_all =  $(hal_OBJS) $(OBJ_REVISION)

.PHONY: all install uninstall clean mrproper \
	pcie_driver pcie_driver_install pcie_driver_uninstall pcie_driver_clean pcie_driver_check \
	libclient libclient_install libclient_uninstall libclient_clean libclient_mrproper \
	libmdp libmdp_install libmdp_uninstall libmdp_clean libmdp_mrproper \
	libbsmp libbsmp_install libbsmp_uninstall libbsmp_clean libbsmp_mrproper \
	hal_install hal_uninstall hal_clean hal_mrproper \
	tests tests_clean tests_mrproper \
	examples examples_clean examples_mrproper \
	cfg cfg_install cfg_uninstall cfg_clean cfg_mrproper

# Avoid deletion of intermediate files, such as objects
.SECONDARY: $(OBJS_all)

# Makefile rules
all: pcie_driver libmdp libbsmp libclient cfg $(OUT)

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

pcie_driver: pcie_driver_check
	$(MAKE) -C $(PCIE_DRIVER_DIR) all

#Verify if the driver is in place
pcie_driver_check:
ifeq ($(wildcard $(DRIVER_OBJ)),)
	@echo "PCI driver not found!";
	@echo "Compilation will continue, but you must install";
	@echo "and load the driver prior to initializing the software";
	@sleep 2;
endif

pcie_driver_install:
	$(MAKE) -C $(PCIE_DRIVER_DIR) install

pcie_driver_uninstall:
	$(MAKE) -C $(PCIE_DRIVER_DIR) uninstall

pcie_driver_clean:
	$(MAKE) -C $(PCIE_DRIVER_DIR) clean

libmdp_pre:
ifeq ($(wildcard $(LIBMDP_DIR)/Makefile),)
	@echo "LIBMDP is not configured. Configuring ..."
	@cd $(LIBMDP_DIR) && \
	    ./autogen.sh && \
	    ./configure
endif

libmdp: libmdp_pre
	$(MAKE) -C $(LIBMDP_DIR)

libmdp_install: libmdp_pre
	$(MAKE) -C $(LIBMDP_DIR) install
	ldconfig

libmdp_uninstall: libmdp_pre
	$(MAKE) -C $(LIBMDP_DIR) install

libmdp_clean: libmdp_pre
	$(MAKE) -C $(LIBMDP_DIR) clean

libmdp_mrproper: libmdp_pre
	$(MAKE) -C $(LIBMDP_DIR) distclean

libbsmp:
	$(MAKE) -C $(LIBBSMP_DIR) all

libbsmp_install:
	$(MAKE) -C $(LIBBSMP_DIR) PREFIX=$(INSTALL_DIR) install

libbsmp_uninstall:
	$(MAKE) -C $(LIBBSMP_DIR) PREFIX=$(INSTALL_DIR) uninstall

libbsmp_clean:
	$(MAKE) -C $(LIBBSMP_DIR) clean

libbsmp_mrproper:
	$(MAKE) -C $(LIBBSMP_DIR) distclean

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
	$(foreach hal_bin,$(ALL_OUT),install -m 755 $(hal_bin) $(INSTALL_DIR)/bin $(CMDSEP))
	$(foreach hal_script,$(INIT_SCRIPTS),install -m 755 $(hal_script) $(INSTALL_DIR)/etc $(CMDSEP))

hal_uninstall:
	$(foreach hal_bin,$(ALL_OUT),rm -f $(INSTALL_DIR)/bin/$(hal_bin) $(CMDSEP))
	$(foreach hal_script,$(INIT_SCRIPTS),rm -f $(INSTALL_DIR)/etc/$(hal_script) $(CMDSEP))

hal_clean:
	rm -f $(OBJS_all) $(OBJS_all:.o=.d)

hal_mrproper:
	rm -f $(ALL_OUT)

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

cfg:
	$(MAKE) -C cfg all

cfg_install:
	$(MAKE) -C cfg install

cfg_uninstall:
	$(MAKE) -C cfg uninstall

cfg_clean:
	$(MAKE) -C cfg clean

cfg_mrproper:
	$(MAKE) -C cfg all

install: hal_install pcie_driver_install libclient_install libmdp_install libbsmp_install cfg_install

uninstall: hal_uninstall pcie_driver_uninstall libclient_uninstall libmdp_uninstall libbsmp_uninstall cfg_uninstall

clean: hal_clean pcie_driver_clean libclient_clean examples_clean tests_clean libmdp_clean libbsmp_clean cfg_clean

mrproper: clean hal_mrproper libclient_mrproper examples_mrproper libclient_mrproper libmdp_mrproper libbsmp_mrproper cfg_mrproper

