# Set your cross compile prefix with CROSS_COMPILE variable
CROSS_COMPILE ?=

CMDSEP = ;

CC ?=		$(CROSS_COMPILE)gcc
AR ?=		$(CROSS_COMPILE)ar
LD ?=		$(CROSS_COMPILE)ld
OBJDUMP ?=	$(CROSS_COMPILE)objdump
OBJCOPY ?=	$(CROSS_COMPILE)objcopy
SIZE ?=		$(CROSS_COMPILE)size
MAKE ?=		make
DEPMOD ?=	depmod

# Select board in which we will work. Options are: ml605 or afcv3
BOARD ?= afcv3_1
# Select which application we want to generate. Options are: halcsd
APPS ?= halcsd
# Select if we want to have the AFCv3 DDR memory shrink to 2^28 or the full size 2^32. Options are: (y)es ot (n)o.
# This is a TEMPORARY fix until the AFCv3 FPGA firmware is fixed. If unsure, select (y)es.
SHRINK_AFCV3_DDR_SIZE ?= n
#Select if we want to compile code with all messages outputs. Options are: y(es) or n(o)
LOCAL_MSG_DBG ?= n
#Select if we want to compile with debug mode on. Options are: y(es) or n(o)
ERRHAND_DBG=y
export ERRHAND_DBG
# Select the minimum debug verbosity. See liberrhand file errhand_opts.h for more info.
ERRHAND_MIN_LEVEL=DBG_LVL_WARN
export ERRHAND_MIN_LEVEL
# Select the subsytems which will have the debug on. See liberrhand file errhand_opts.h for more info.
ERRHAND_SUBSYS_ON="(DBG_DEV_MNGR | DBG_DEV_IO | DBG_SM_IO | DBG_LIB_CLIENT | DBG_SM_PR | DBG_SM_CH | DBG_LL_IO | DBG_HAL_UTILS)"
export ERRHAND_SUBSYS_ON
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
# Selects if we want to compile specfic APP Config. Options are: y(es) or n(o).
# If selected, the FPGA firmware must have the AFC diagnostics module
# synthesized.
WITH_APP_CFG ?= n
# Installation prefix for the scripts. This is mainly used for testing the build
# system. Usually this is empty
SCRIPTS_PREFIX ?=
# Selects the install location of the config file
PREFIX ?= /usr/local
export PREFIX

# All of our supported boards
SUPPORTED_ML605_BOARDS = ml605
export SUPPORTED_ML605_BOARDS
SUPPORTED_AFCV3_BOARDS = afcv3 afcv3_1
export SUPPORTED_AFCV3_BOARDS

# Top Makefile directory
TOP := $(strip $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST)))))
SRC_DIR := $(TOP)/src
export TOP

# Libraries
LIBS_DIR := $(TOP)/libs
export LIBS_DIR

# Subdmoules and third-party codes
FOREIGN_DIR := $(TOP)/foreign
export FOREIGN_DIR

# Linker script
LD_SCRIPT = $(TOP)/linker/halcs.ld

# Init sripts
INIT_SCRIPTS =

# PCIe driver stuff (pcie driver and library) relative
# directory
KERNEL_VERSION ?= $(shell uname -r)
DRIVER_OBJ = /lib/modules/$(KERNEL_VERSION)/extra/pciDriver.ko

# Project libraries
LIBERRHAND_DIR = $(LIBS_DIR)/errhand
export LIBERRHAND_DIR
LIBCONVC_DIR = $(LIBS_DIR)/convc
export LIBCONVC_DIR
LIBHUTILS_DIR = $(LIBS_DIR)/hutils
export LIBHUTILS_DIR
LIBDISPTABLE_DIR = $(LIBS_DIR)/disptable
export LIBDISPTABLE_DIR
LIBLLIO_DIR = $(LIBS_DIR)/llio
export LIBLLIO_DIR
LIBHALCSCLIENT_DIR = $(LIBS_DIR)/halcsclient
export LIBHALCSCLIENT_DIR
LIBACQCLIENT_DIR = $(LIBS_DIR)/acqclient
export LIBACQCLIENT_DIR
LIBBPMCLIENT_DIR = $(LIBS_DIR)/bpmclient
export LIBBPMCLIENT_DIR
LIBSDBUTILS_DIR = $(LIBS_DIR)/sdbutils
export LIBSDBUTILS_DIR
# Our submodules and third-party codes
LIBSDBFS_DIR = $(FOREIGN_DIR)/libsdbfs
export LIBSDBFS_DIR
LIBBSMP_DIR = $(FOREIGN_DIR)/libbsmp
export LIBBSMP_DIR
PCIE_DRIVER_DIR = $(FOREIGN_DIR)/pcie-driver
export PCIE_DRIVER_DIR

# General C/CPP flags
CFLAGS_USR = -std=gnu99 -O2
# We expect tghese variables to be appended to the possible
# command-line options
override CPPFLAGS +=
override CXXFLAGS +=

ifeq ($(BOARD),$(filter $(BOARD),$(SUPPORTED_AFCV3_BOARDS)))
ifeq ($(SHRINK_AFCV3_DDR_SIZE),y)
CFLAGS_USR += -D__SHRINK_AFCV3_DDR_SIZE__
endif
endif

# Board selection
ifeq ($(BOARD),$(filter $(BOARD),$(SUPPORTED_ML605_BOARDS)))
CFLAGS_USR += -D__BOARD_ML605__ -D__WR_SHIFT_FIX__=2
endif

ifeq ($(BOARD),afcv3_1)
CFLAGS_USR += -D__BOARD_AFCV3_1__
endif

ifeq ($(BOARD),$(filter $(BOARD),$(SUPPORTED_AFCV3_BOARDS)))
CFLAGS_USR += -D__BOARD_AFCV3__ -D__WR_SHIFT_FIX__=0
endif

# Program FMC130M_4CH EEPROM
ifeq ($(FMC130M_4CH_EEPROM_PROGRAM),active)
CFLAGS_USR += -D__FMC130M_4CH_EEPROM_PROGRAM__=1
endif

ifeq ($(FMC130M_4CH_EEPROM_PROGRAM),passive)
CFLAGS_USR += -D__FMC130M_4CH_EEPROM_PROGRAM__=2
endif

# Program FMC250M_4CH EEPROM
ifeq ($(FMC250M_4CH_EEPROM_PROGRAM),active)
CFLAGS_USR += -D__FMC250M_4CH_EEPROM_PROGRAM__=1
endif

ifeq ($(FMC250M_4CH_EEPROM_PROGRAM),passive)
CFLAGS_USR += -D__FMC250M_4CH_EEPROM_PROGRAM__=2
endif

# Compile DEV MNGR or not
ifeq ($(WITH_DEV_MNGR),y)
CFLAGS_USR += -D__WITH_DEV_MNGR__
endif

ifeq ($(AFE_RFFE_TYPE),1)
CFLAGS_USR += -D__AFE_RFFE_V1__
endif

ifeq ($(AFE_RFFE_TYPE),2)
CFLAGS_USR += -D__AFE_RFFE_V2__
endif

# Compile APP Config or not
ifeq ($(WITH_APP_CFG),y)
CFLAGS_USR += -D__WITH_APP_CFG__
endif

ifneq ($(CFG_DIR),)
CFLAGS_USR += -D__CFG_DIR__=$(CFG_DIR)
endif

ifneq ($(CFG_FILENAME),)
CFLAGS_USR += -D__CFG_FILENAME__=$(CFG_FILENAME)
endif

# Malamute 1.0.0 requires this to be defined
# as all of its API is in DRAFT state
CFLAGS_USR += -DMLM_BUILD_DRAFT_API

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
LDFLAGS_PLATFORM = -Wl,-T,$(LD_SCRIPT)

# Libraries
LIBS = -lm -lzmq -lczmq -lmlm

# FIXME: make the project libraries easily interchangeable, specifying
# the lib only a single time
PROJECT_LIBS_NAME = liberrhand libconvc libhutils libdisptable libllio libhalcsclient \
                    libacqclient libbpmclient libsdbutils  libsdbfs libpcidriver \
					libpthread
PROJECT_LIBS = -lerrhand -lconvc -lhutils -ldisptable -lllio -lhalcsclient \
               -lacqclient -lbpmclient -lsdbutils -lsdbfs -lpcidriver \
			   -lpthread

# General library flags -L<libdir>
LFLAGS = -L$(LIBSDBFS_DIR) \
	     -L$(LIBBSMP_DIR) \
	     -L$(PCIE_DRIVER_DIR)/lib/pcie

LFLAGS += -L$(LIBERRHAND_DIR) \
	      -L$(LIBCONVC_DIR) \
	      -L$(LIBHUTILS_DIR) \
	      -L$(LIBDISPTABLE_DIR) \
	      -L$(LIBLLIO_DIR) \
	      -L$(LIBHALCSCLIENT_DIR) \
	      -L$(LIBACQCLIENT_DIR) \
	      -L$(LIBBPMCLIENT_DIR) \
	      -L$(LIBSDBUTILS_DIR)

# Specific platform objects
OBJS_PLATFORM =

# Prepare "apps" include
APPS_MKS = $(foreach mk,$(APPS),$(SRC_DIR)/apps/$(mk)/$(mk).mk)

# Include other Makefiles as needed here
include $(SRC_DIR)/sm_io/sm_io.mk
include $(SRC_DIR)/sm_io_table/sm_io_table.mk
include $(SRC_DIR)/dev_mngr/dev_mngr.mk
include $(SRC_DIR)/dev_io/dev_io.mk
include $(SRC_DIR)/msg/msg.mk
include $(SRC_DIR)/revision/revision.mk
include $(SRC_DIR)/boards/$(BOARD)/board.mk
include $(SRC_DIR)/boards/common/common.mk
include $(APPS_MKS)

# Project boards
boards_INCLUDE_DIRS = -Iinclude/boards/$(BOARD)

# We need this until we merge smio and smio_table modules
# together
smio_table_DIRS = $(shell find $(SRC_DIR)/sm_io_table/modules -type d -print)
smio_table_INCLUDE_DIRS = $(addprefix -I, $(smio_table_DIRS))

# Include directories
INCLUDE_DIRS = $(boards_INCLUDE_DIRS) \
	        $(smio_table_INCLUDE_DIRS) \
	       -Iinclude \
	       -I$(LIBBSMP_DIR)/include \
	       -I$(LIBSDBFS_DIR)/include \
           -I$(LIBERRHAND_DIR)/include \
	       -I$(LIBCONVC_DIR)/include \
	       -I$(LIBHUTILS_DIR)/include \
	       -I$(LIBDISPTABLE_DIR)/include \
	       -I$(LIBLLIO_DIR)/include \
	       -I$(LIBHALCSCLIENT_DIR)/include \
	       -I$(LIBACQCLIENT_DIR)/include \
	       -I$(LIBBPMCLIENT_DIR)/include \
	       -I$(LIBSDBUTILS_DIR)/include \
	       -I${PREFIX}/include

# Merge all flags. We expect tghese variables to be appended to the possible
# command-line options
override CFLAGS += $(CFLAGS_USR) $(CFLAGS_PLATFORM) $(CFLAGS_DEBUG) $(CPPFLAGS) $(CXXFLAGS)
override LDFLAGS += $(LDFLAGS_PLATFORM)

# Output modules
OUT = $(dev_mngr_OUT)
# Get each APP OUT module
OUT += $(foreach out,$(APPS),$($(out)_OUT))

# All possible output modules
ALL_OUT = $(dev_mngr_all_OUT)
# Get each APP all possible output modules
ALL_OUT += $(foreach all_out,$(APPS),$($(all_out)_all_OUT))

# Out objects
dev_mngr_OBJS += $(dev_mngr_core_OBJS) $(debug_OBJS) \
                 $(exp_ops_OBJS) $(thsafe_msg_zmq_OBJS) \
                 $(ll_io_utils_OBJS) $(dev_io_core_utils_OBJS)

common_app_OBJS = $(dev_io_core_OBJS) $(ll_io_OBJS) \
               $(sm_io_OBJS) $(sm_io_table_OBJS) $(msg_OBJS) \
               $(board_OBJS) $(board_common_OBJS)

apps_OBJS = $(foreach app_obj,$(APPS),$($(app_obj)_all_OBJS))

apps_SCRIPTS = $(foreach app,$(APPS),$($(app)_SCRIPTS))

.SECONDEXPANSION:

# Save a git repository description
GIT_REVISION = $(shell git describe --dirty --always)
GIT_USER_NAME = $(shell git config --get user.name)
GIT_USER_EMAIL = $(shell git config --get user.email)

OBJS_all = $(ll_io_OBJS) \
	   $(sm_io_OBJS) \
	   $(sm_io_table_OBJS) \
	   $(msg_OBJS) \
	   $(dev_mngr_OBJS) \
	   $(common_app_OBJS) \
	   $(apps_OBJS) \
	   $(board_common_OBJS) \
	   $(revision_OBJS)

# Sources
all_SRCS = $(patsubst %.o,%.c,$(OBJS_all))
revision_SRCS = $(patsubst %.o,%.c,$(revision_OBJS))

.PHONY: all install uninstall clean mrproper \
	pcie_driver pcie_driver_install pcie_driver_uninstall pcie_driver_clean pcie_driver_check \
	lib_pcie_driver lib_pcie_driver_install lib_pcie_driver_uninstall lib_pcie_driver_clean lib_pcie_driver_mrproper \
	liberrhand liberrhand_install liberrhand_uninstall liberrhand_clean liberrhand_mrproper \
	libconvc libconvc_install libconvc_uninstall libconvc_clean libconvc_mrproper \
	libhutils libhutils_install libhutils_uninstall libhutils_clean libhutils_mrproper \
	libdisptable libdisptable_install libdisptable_uninstall libdisptable_clean libdisptable_mrproper \
	libllio libllio_install libllio_uninstall libllio_clean libllio_mrproper \
	libhalcsclient libhalcsclient_install libhalcsclient_uninstall libhalcsclient_clean libhalcsclient_mrproper \
	libacqclient libacqclient_install libacqclient_uninstall libacqclient_clean libacqclient_mrproper \
	libbpmclient libbpmclient_install libbpmclient_uninstall libbpmclient_clean libbpmclient_mrproper \
	libsdbutils libsdbutils_install libsdbutils_uninstall libsdbutils_clean libsdbutils_mrproper \
	libsdbfs libsdbfs_install libsdbfs_uninstall libsdbfs_clean libsdbfs_mrproper \
	libbsmp libbsmp_install libbsmp_uninstall libbsmp_clean libbsmp_mrproper \
	core_install core_uninstall core_clean core_mrproper \
	tests tests_clean tests_mrproper \
	examples examples_clean examples_mrproper

# Avoid deletion of intermediate files, such as objects
.SECONDARY: $(OBJS_all)

# Makefile rules
all: deps libs $(OUT)

# Output Rule
$(OUT): $$($$@_OBJS) $(common_app_OBJS) $(revision_OBJS)
	$(CC) $(LDFLAGS) $(LFLAGS) $(CFLAGS) $(INCLUDE_DIRS) -o $@ $^ $($@_STATIC_LIBS) $(LIBS) $($@_LIBS) $(PROJECT_LIBS)

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

# Install just the driver and lib, not udev rules
pcie_driver_install:
	$(MAKE) -C $(PCIE_DRIVER_DIR) core_driver_install lib_driver_install
	$(DEPMOD) -a $(KERNEL_VERSION)

pcie_driver_uninstall:
	$(MAKE) -C $(PCIE_DRIVER_DIR) core_driver_uninstall lib_driver_uninstall

pcie_driver_clean:
	$(MAKE) -C $(PCIE_DRIVER_DIR) clean

lib_pcie_driver:
	$(MAKE) -C $(PCIE_DRIVER_DIR) lib_driver

lib_pcie_driver_install:
	$(MAKE) -C $(PCIE_DRIVER_DIR) PREFIX=${PREFIX} lib_driver_install

lib_pcie_driver_uninstall:
	$(MAKE) -C $(PCIE_DRIVER_DIR) PREFIX=${PREFIX} lib_driver_uninstall

lib_pcie_driver_clean:
	$(MAKE) -C $(PCIE_DRIVER_DIR) lib_driver_clean

lib_pcie_driver_mrproper: lib_pcie_driver_clean

libbsmp:
	$(MAKE) -C $(LIBBSMP_DIR) all

libbsmp_install:
	$(MAKE) -C $(LIBBSMP_DIR) PREFIX=${PREFIX} install

libbsmp_uninstall:
	$(MAKE) -C $(LIBBSMP_DIR) PREFIX=${PREFIX} uninstall

libbsmp_clean:
	$(MAKE) -C $(LIBBSMP_DIR) clean

libbsmp_mrproper:
	$(MAKE) -C $(LIBBSMP_DIR) distclean

# Project Libraries

liberrhand:
	$(MAKE) -C $(LIBERRHAND_DIR) all

liberrhand_install:
	$(MAKE) -C $(LIBERRHAND_DIR) PREFIX=${PREFIX} install

liberrhand_uninstall:
	$(MAKE) -C $(LIBERRHAND_DIR) PREFIX=${PREFIX} uninstall

liberrhand_clean:
	$(MAKE) -C $(LIBERRHAND_DIR) clean

liberrhand_mrproper:
	$(MAKE) -C $(LIBERRHAND_DIR) mrproper

libconvc:
	$(MAKE) -C $(LIBCONVC_DIR) all

libconvc_install:
	$(MAKE) -C $(LIBCONVC_DIR) PREFIX=${PREFIX} install

libconvc_uninstall:
	$(MAKE) -C $(LIBCONVC_DIR) PREFIX=${PREFIX} uninstall

libconvc_clean:
	$(MAKE) -C $(LIBCONVC_DIR) clean

libconvc_mrproper:
	$(MAKE) -C $(LIBCONVC_DIR) mrproper

libhutils:
	$(MAKE) -C $(LIBHUTILS_DIR) all

libhutils_install:
	$(MAKE) -C $(LIBHUTILS_DIR) PREFIX=${PREFIX} install

libhutils_uninstall:
	$(MAKE) -C $(LIBHUTILS_DIR) PREFIX=${PREFIX} uninstall

libhutils_clean:
	$(MAKE) -C $(LIBHUTILS_DIR) clean

libhutils_mrproper:
	$(MAKE) -C $(LIBHUTILS_DIR) mrproper

libdisptable:
	$(MAKE) -C $(LIBDISPTABLE_DIR) all

libdisptable_install:
	$(MAKE) -C $(LIBDISPTABLE_DIR) PREFIX=${PREFIX} install

libdisptable_uninstall:
	$(MAKE) -C $(LIBDISPTABLE_DIR) PREFIX=${PREFIX} uninstall

libdisptable_clean:
	$(MAKE) -C $(LIBDISPTABLE_DIR) clean

libdisptable_mrproper:
	$(MAKE) -C $(LIBDISPTABLE_DIR) mrproper

libllio:
	$(MAKE) -C $(LIBLLIO_DIR) all

libllio_install:
	$(MAKE) -C $(LIBLLIO_DIR) PREFIX=${PREFIX} install

libllio_uninstall:
	$(MAKE) -C $(LIBLLIO_DIR) PREFIX=${PREFIX} uninstall

libllio_clean:
	$(MAKE) -C $(LIBLLIO_DIR) clean

libllio_mrproper:
	$(MAKE) -C $(LIBLLIO_DIR) mrproper

libhalcsclient:
	$(MAKE) -C $(LIBHALCSCLIENT_DIR) all

libhalcsclient_install:
	$(MAKE) -C $(LIBHALCSCLIENT_DIR) PREFIX=${PREFIX} install

libhalcsclient_uninstall:
	$(MAKE) -C $(LIBHALCSCLIENT_DIR) PREFIX=${PREFIX} uninstall

libhalcsclient_clean:
	$(MAKE) -C $(LIBHALCSCLIENT_DIR) clean

libhalcsclient_mrproper:
	$(MAKE) -C $(LIBHALCSCLIENT_DIR) mrproper

libacqclient:
	$(MAKE) -C $(LIBACQCLIENT_DIR) all

libacqclient_install:
	$(MAKE) -C $(LIBACQCLIENT_DIR) PREFIX=${PREFIX} install

libacqclient_uninstall:
	$(MAKE) -C $(LIBACQCLIENT_DIR) PREFIX=${PREFIX} uninstall

libacqclient_clean:
	$(MAKE) -C $(LIBACQCLIENT_DIR) clean

libacqclient_mrproper:
	$(MAKE) -C $(LIBACQCLIENT_DIR) mrproper

libbpmclient:
	$(MAKE) -C $(LIBBPMCLIENT_DIR) all

libbpmclient_install:
	$(MAKE) -C $(LIBBPMCLIENT_DIR) PREFIX=${PREFIX} install

libbpmclient_uninstall:
	$(MAKE) -C $(LIBBPMCLIENT_DIR) PREFIX=${PREFIX} uninstall

libbpmclient_clean:
	$(MAKE) -C $(LIBBPMCLIENT_DIR) clean

libbpmclient_mrproper:
	$(MAKE) -C $(LIBBPMCLIENT_DIR) mrproper

libsdbutils:
	$(MAKE) -C $(LIBSDBUTILS_DIR) all

libsdbutils_install:
	$(MAKE) -C $(LIBSDBUTILS_DIR) PREFIX=${PREFIX} install

libsdbutils_uninstall:
	$(MAKE) -C $(LIBSDBUTILS_DIR) PREFIX=${PREFIX} uninstall

libsdbutils_clean:
	$(MAKE) -C $(LIBSDBUTILS_DIR) clean

libsdbutils_mrproper:
	$(MAKE) -C $(LIBSDBUTILS_DIR) mrproper

libsdbfs:
	$(MAKE) -C $(LIBSDBFS_DIR) all

libsdbfs_install:
	$(MAKE) -C $(LIBSDBFS_DIR) PREFIX=${PREFIX} install

libsdbfs_uninstall:
	$(MAKE) -C $(LIBSDBFS_DIR) PREFIX=${PREFIX} uninstall

libsdbfs_clean:
	$(MAKE) -C $(LIBSDBFS_DIR) clean

libsdbfs_mrproper:
	$(MAKE) -C $(LIBSDBFS_DIR) mrproper

libs: liberrhand libconvc libhutils \
    libdisptable libllio libhalcsclient libacqclient libbpmclient libsdbutils \
    libsdbfs

libs_install: liberrhand_install libconvc_install libhutils_install \
    libdisptable_install libllio_install libhalcsclient_install \
    libacqclient_install libbpmclient_install libsdbutils_install \
    libsdbfs_install

libs_compile_install: liberrhand liberrhand_install libconvc libconvc_install \
    libhutils libhutils_install libdisptable libdisptable_install libllio libllio_install \
    libhalcsclient libhalcsclient_install libacqclient libacqclient_install \
    libbpmclient libbpmclient_install libsdbutils libsdbutils_install \
    libsdbfs libsdbfs_install

libs_uninstall: liberrhand_uninstall libconvc_uninstall libhutils_uninstall \
    libdisptable_uninstall libllio_uninstall libhalcsclient_uninstall \
    libacqclient_uninstall libbpmclient_uninstall libsdbutils_uninstall \
    libsdbfs_uninstall

libs_clean: liberrhand_clean libconvc_clean libhutils_clean \
    libdisptable_clean libllio_clean libhalcsclient_clean libacqclient_clean \
    libbpmclient_clean libsdbutils_clean libsdbfs_clean

libs_mrproper: liberrhand_mrproper libconvc_mrproper libhutils_mrproper \
    libdisptable_mrproper libllio_mrproper libhalcsclient_mrproper \
    libacqclient_mrproper libbpmclient_mrproper libsdbutils_mrproper \
    libsdbfs_mrproper

# External project dependencies

deps: libbsmp lib_pcie_driver

deps_install: libbsmp_install lib_pcie_driver_install

deps_uninstall: libbsmp_uninstall lib_pcie_driver_uninstall

deps_clean: libbsmp_clean lib_pcie_driver_clean

deps_mrproper: libbsmp_mrproper lib_pcie_driver_mrproper

core_install:
	$(foreach core_bin,$(OUT),install -m 755 $(core_bin) ${PREFIX}/bin $(CMDSEP))

core_uninstall:
	$(foreach core_bin,$(ALL_OUT),rm -f ${PREFIX}/bin/$(core_bin) $(CMDSEP))

core_clean:
	rm -f $(OBJS_all) $(OBJS_all:.o=.d)

core_mrproper:
	rm -f $(ALL_OUT)

scripts_install:
	$(foreach app_script,$(apps_SCRIPTS),mkdir -p $(dir ${SCRIPTS_PREFIX}/$(shell echo $(app_script) | cut -f2 -d:)) $(CMDSEP))
	$(foreach app_script,$(apps_SCRIPTS),cp --preserve=mode $(subst :,,$(app_script)) ${SCRIPTS_PREFIX}/$(shell echo $(app_script) | cut -f2 -d:) $(CMDSEP))
	$(MAKE) -C scripts SCRIPTS_PREFIX=${SCRIPTS_PREFIX} install

scripts_uninstall:
	$(foreach app_script,$(apps_SCRIPTS),rm -f ${SCRIPTS_PREFIX}/$(shell echo $(app_script) | cut -f2 -d:) $(CMDSEP))
	$(MAKE) -C scripts SCRIPTS_PREFIX=${SCRIPTS_PREFIX} uninstall

scripts_clean:
	$(MAKE) -C scripts clean

scripts_mrproper:
	$(MAKE) -C scripts mrproper

tests:
	$(MAKE) -C tests all

tests_clean:
	$(MAKE) -C tests clean

tests_mrproper:
	$(MAKE) -C tests mrproper

examples:
	$(MAKE) -C examples all

examples_install:
	$(MAKE) -C examples install

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

install: core_install deps_install liberrhand_install libconvc_install \
    libsdbutils_install libhutils_install libdisptable_install libllio_install \
    libhalcsclient_install libacqclient_install libbpmclient_install \
    scripts_install

uninstall: core_uninstall deps_uninstall liberrhand_uninstall libconvc_uninstall \
    libsdbutils_uninstall libhutils_uninstall libdisptable_uninstall libllio_uninstall \
    libhalcsclient_uninstall libacqclient_uninstall libbpmclient_uninstall \
    scripts_uninstall

clean: core_clean deps_clean liberrhand_clean libconvc_clean libsdbutils_clean \
    libhutils_clean libdisptable_clean libllio_clean libhalcsclient_clean \
    libacqclient_clean libbpmclient_clean examples_clean tests_clean \
    scripts_clean

mrproper: clean core_mrproper deps_mrproper liberrhand_mrproper libconvc_mrproper \
    libsdbutils_mrproper libhutils_mrproper libdisptable_mrproper libllio_mrproper \
    libhalcsclient_mrproper libacqclient_mrproper libbpmclient_mrproper \
    examples_mrproper tests_mrproper scripts_mrproper
