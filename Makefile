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
# Selects if we want to compile DEVIO Config. Options are: y(es) or n(o).
# If selected, the FPGA firmware must have the AFC diagnostics module
# synthesized.
WITH_DEVIO_CFG ?= y
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

# Project libraries
LIBERRHAND_DIR = src/libs/liberrhand
LIBCONVC_DIR = src/libs/libconvc
LIBHUTILS_DIR = src/libs/libhutils
LIBDISPTABLE_DIR = src/libs/libdisptable
LIBBPMCLIENT_DIR = src/libs/libbpmclient
LIBSDBFS_DIR = foreign/libsdbfs

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

# Compile DEVIO Config or not
ifeq ($(WITH_DEVIO_CFG),y)
CFLAGS += -D__WITH_DEVIO_CFG__
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

# To enable this option, use: make ERRHAND_DBG=y
ifneq ($(ERRHAND_DBG),)
CFLAGS_DEBUG += -DERRHAND_DBG=$(ERRHAND_DBG)
endif

# To enable this option use: make ERRHAND_MIN_LEVEL=DBG_MIN_TRACE
ifneq ($(ERRHAND_MIN_LEVEL),)
CFLAGS_DEBUG += -DERRHAND_MIN_LEVEL=$(ERRHAND_MIN_LEVEL)
endif

# To enable this option use: make ERRHAND_SUBSYS_ON='"(DBG_DEV_MNGR | \
# DBG_DEV_IO | DBG_SM_IO | DBG_LIB_CLIENT  | DBG_SM_PR | DBG_SM_CH | DBG_LL_IO | DBG_HAL_UTILS)"'
#
# You can also OR the available subsytems to enable debug messages in just the
# those subsytems. See file errhand_opts.h for more information
ifneq ($(ERRHAND_SUBSYS_ON),)
CFLAGS_DEBUG += -DERRHAND_SUBSYS_ON=$(ERRHAND_SUBSYS_ON)
endif

# Debug flags -D<flasg_name>=<value>
CFLAGS_DEBUG += -g

# Specific platform Flags
CFLAGS_PLATFORM = -Wall -Wextra -Werror
LDFLAGS_PLATFORM =

# Libraries
LIBS = -lm -lzmq -lczmq -lmdp -lpcidriver

# FIXME: make the project libraries easily interchangeable, specifying
# the lib only a single time
PROJECT_LIBS_NAME = liberrhand libconvc libhutils libdisptable libbpmclient libsdbfs
PROJECT_LIBS = -lerrhand -lconvc -lhutils -ldisptable -lbpmclient -lsdbfs

# General library flags -L<libdir>
LFLAGS = -Lsrc/libs/liberrhand -Lsrc/libs/libconvc -Lsrc/libs/libhutils \
         -Lsrc/libs/libdisptable -Lsrc/libs/libbpmclient -Lforeign/libsdbfs

# Specific platform objects
OBJS_PLATFORM =

# Source directory
SRC_DIR = src

# Include other Makefiles as needed here
include $(SRC_DIR)/ll_io/ll_io.mk
include $(SRC_DIR)/sm_io/sm_io.mk
include $(SRC_DIR)/dev_mngr/dev_mngr.mk
include $(SRC_DIR)/dev_io/dev_io.mk
include $(SRC_DIR)/msg/msg.mk
include $(SRC_DIR)/revision/revision.mk

# Project boards
boards_INCLUDE_DIRS = -Iinclude/boards/$(BOARD)

# Include directories
INCLUDE_DIRS =  \
	       $(ll_io_INCLUDE_DIRS) \
	       $(sm_io_INCLUDE_DIRS) \
	       $(msg_INCLUDE_DIRS) \
	       $(dev_mngr_INCLUDE_DIRS) \
	       $(dev_io_INCLUDE_DIRS) \
	       $(revision_INCLUDE_DIRS) \
	       $(boards_INCLUDE_DIRS) \
	       -I$(PCIE_DRIVER_DIR)/include/pcie \
	       -Iinclude \
	       -Isrc/libs/liberrhand \
	       -Isrc/libs/libconvc \
	       -Isrc/libs/libhutils \
	       -Isrc/libs/libdisptable \
	       -Isrc/libs/libbpmclient \
	       -Iforeign/libsdbfs \
	       -I/usr/local/include

# Merge all flags.
CFLAGS += $(CFLAGS_PLATFORM) $(CFLAGS_DEBUG)

LDFLAGS = $(LDFLAGS_PLATFORM)

# Output modules
OUT = $(dev_mngr_OUT) $(dev_io_OUT)

# All possible output modules
ALL_OUT = $(dev_mngr_all_OUT) $(dev_io_all_OUT)

# Out objects
dev_mngr_OBJS += $(dev_mngr_core_OBJS) $(debug_OBJS) \
                 $(exp_ops_OBJS) $(thsafe_msg_zmq_OBJS) \
                 $(ll_io_utils_OBJS) $(dev_io_core_utils_OBJS)

dev_io_OBJS += $(dev_io_core_OBJS) $(ll_io_OBJS) \
               $(sm_io_OBJS) $(msg_OBJS)

dev_io_cfg_OBJS += $(dev_io_core_OBJS) $(ll_io_OBJS) \
                   $(sm_io_OBJS) $(msg_OBJS)

# Specific libraries for OUT targets
dev_mngr_LIBS =
dev_mngr_STATIC_LIBS =

dev_io_LIBS = -lbsmp
dev_io_STATIC_LIBS =

dev_io_cfg_LIBS = -lbsmp
dev_io_cfg_STATIC_LIBS =

.SECONDEXPANSION:

# Save a git repository description
GIT_REVISION = $(shell git describe --dirty --always)
GIT_USER_NAME = $(shell git config --get user.name)
GIT_USER_EMAIL = $(shell git config --get user.email)

OBJS_all = $(ll_io_OBJS) \
	   $(sm_io_OBJS) \
	   $(msg_OBJS) \
	   $(dev_mngr_OBJS) \
	   $(dev_io_OBJS) \
	   $(dev_io_cfg_OBJS) \
	   $(revision_OBJS)

# Sources
all_SRCS = $(patsubst %.o,%.c,$(OBJS_all))
revision_SRCS = $(patsubst %.o,%.c,$(revision_OBJS))

.PHONY: all install uninstall clean mrproper \
	pcie_driver pcie_driver_install pcie_driver_uninstall pcie_driver_clean pcie_driver_check \
	liberrhand liberrhand_install liberrhand_uninstall liberrhand_clean liberrhand_mrproper \
	libconvc libconvc_install libconvc_uninstall libconvc_clean libconvc_mrproper \
	libhutils libhutils_install libhutils_uninstall libhutils_clean libhutils_mrproper \
	libdisptable libdisptable_install libdisptable_uninstall libdisptable_clean libdisptable_mrproper \
	libbpmclient libbpmclient_install libbpmclient_uninstall libbpmclient_clean libbpmclient_mrproper \
	libsdbfs libsdbfs_install libsdbfs_uninstall libsdbfs_clean libsdbfs_mrproper \
	libmdp libmdp_install libmdp_uninstall libmdp_clean libmdp_mrproper \
	libbsmp libbsmp_install libbsmp_uninstall libbsmp_clean libbsmp_mrproper \
	core_install core_uninstall core_clean core_mrproper \
	tests tests_clean tests_mrproper \
	examples examples_clean examples_mrproper \
	cfg cfg_install cfg_uninstall cfg_clean cfg_mrproper

# Avoid deletion of intermediate files, such as objects
.SECONDARY: $(OBJS_all)

# Makefile rules
all: $(PROJECT_LIBS_NAME) cfg $(OUT)

# Output Rule
$(OUT): $$($$@_OBJS) $(revision_OBJS)
	$(CC) $(LFLAGS) $(CFLAGS) $(INCLUDE_DIRS) -o $@ $^ $($@_STATIC_LIBS) $(LDFLAGS) $(LIBS) $($@_LIBS) $(PROJECT_LIBS)

# Special rule for the revision object
$(revision_OBJS): $(revision_SRCS)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -DGIT_REVISION="\"$(GIT_REVISION)\"" \
        -DGIT_USER_NAME="\"$(GIT_USER_NAME)\"" \
        -DGIT_USER_EMAIL="\"$(GIT_USER_EMAIL)\"" \
        -c $< -o $@

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

pcie_driver:
	$(MAKE) -C $(PCIE_DRIVER_DIR) all

#Verify if the driver is in place
pcie_driver_check:
ifeq ($(wildcard $(DRIVER_OBJ)),)
	@echo "PCI driver not found!";
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
	$(MAKE) -C $(LIBMDP_DIR) uninstall

libmdp_clean:
ifneq ($(wildcard $(LIBMDP_DIR)/Makefile),)
	$(MAKE) -C $(LIBMDP_DIR) clean
endif

libmdp_mrproper:
ifneq ($(wildcard $(LIBMDP_DIR)/Makefile),)
	$(MAKE) -C $(LIBMDP_DIR) distclean
endif

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

# Project Libraries

liberrhand:
	$(MAKE) -C $(LIBERRHAND_DIR) all

liberrhand_install:
	$(MAKE) -C $(LIBERRHAND_DIR) install

liberrhand_uninstall:
	$(MAKE) -C $(LIBERRHAND_DIR) uninstall

liberrhand_clean:
	$(MAKE) -C $(LIBERRHAND_DIR) clean

liberrhand_mrproper:
	$(MAKE) -C $(LIBERRHAND_DIR) mrproper

libconvc:
	$(MAKE) -C $(LIBCONVC_DIR) all

libconvc_install:
	$(MAKE) -C $(LIBCONVC_DIR) install

libconvc_uninstall:
	$(MAKE) -C $(LIBCONVC_DIR) uninstall

libconvc_clean:
	$(MAKE) -C $(LIBCONVC_DIR) clean

libconvc_mrproper:
	$(MAKE) -C $(LIBCONVC_DIR) mrproper

libhutils:
	$(MAKE) -C $(LIBHUTILS_DIR) all

libhutils_install:
	$(MAKE) -C $(LIBHUTILS_DIR) install

libhutils_uninstall:
	$(MAKE) -C $(LIBHUTILS_DIR) uninstall

libhutils_clean:
	$(MAKE) -C $(LIBHUTILS_DIR) clean

libhutils_mrproper:
	$(MAKE) -C $(LIBHUTILS_DIR) mrproper

libdisptable:
	$(MAKE) -C $(LIBDISPTABLE_DIR) all

libdisptable_install:
	$(MAKE) -C $(LIBDISPTABLE_DIR) install

libdisptable_uninstall:
	$(MAKE) -C $(LIBDISPTABLE_DIR) uninstall

libdisptable_clean:
	$(MAKE) -C $(LIBDISPTABLE_DIR) clean

libdisptable_mrproper:
	$(MAKE) -C $(LIBDISPTABLE_DIR) mrproper

libbpmclient:
	$(MAKE) -C $(LIBBPMCLIENT_DIR) all

libbpmclient_install:
	$(MAKE) -C $(LIBBPMCLIENT_DIR) install

libbpmclient_uninstall:
	$(MAKE) -C $(LIBBPMCLIENT_DIR) uninstall

libbpmclient_clean:
	$(MAKE) -C $(LIBBPMCLIENT_DIR) clean

libbpmclient_mrproper:
	$(MAKE) -C $(LIBBPMCLIENT_DIR) mrproper

libsdbfs:
	$(MAKE) -C $(LIBSDBFS_DIR) all

libsdbfs_install:
	$(MAKE) -C $(LIBSDBFS_DIR) install

libsdbfs_uninstall:
	$(MAKE) -C $(LIBSDBFS_DIR) uninstall

libsdbfs_clean:
	$(MAKE) -C $(LIBSDBFS_DIR) clean

libsdbfs_mrproper:
	$(MAKE) -C $(LIBSDBFS_DIR) mrproper

# External project dependencies

deps: libmdp libbsmp

deps_install: libmdp_install libbsmp_install

deps_uninstall: libmdp_uninstall libbsmp_uninstall

deps_clean: libmdp_clean libbsmp_clean

deps_mrproper: libmdp_mrproper libbsmp_mrproper

core_install:
	$(foreach core_bin,$(OUT),install -m 755 $(core_bin) $(INSTALL_DIR)/bin $(CMDSEP))
	$(foreach core_script,$(INIT_SCRIPTS),install -m 755 $(core_script) $(INSTALL_DIR)/etc $(CMDSEP))

core_uninstall:
	$(foreach core_bin,$(ALL_OUT),rm -f $(INSTALL_DIR)/bin/$(core_bin) $(CMDSEP))
	$(foreach core_script,$(INIT_SCRIPTS),rm -f $(INSTALL_DIR)/etc/$(core_script) $(CMDSEP))

core_clean:
	rm -f $(OBJS_all) $(OBJS_all:.o=.d)

core_mrproper:
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
	$(MAKE) -C cfg mrproper

install: core_install deps_install liberrhand_install libconvc_install libhutils_install libbpmclient_install cfg_install

uninstall: core_uninstall deps_uninstall liberrhand_uninstall libconvc_uninstall libhutils_uninstall libbpmclient_uninstall cfg_uninstall

clean: core_clean deps_clean liberrhand_clean libconvc_clean libhutils_clean libbpmclient_clean examples_clean tests_clean cfg_clean

mrproper: clean core_mrproper deps_mrproper liberrhand_mrproper libconvc_mrproper libhutils_mrproper libbpmclient_mrproper examples_mrproper tests_mrproper cfg_mrproper

