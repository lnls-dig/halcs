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

TOP ?= ../..

PWD = 		$(shell pwd)

LIBNAME_RAW = halcsclient
LIBNAME = lib$(LIBNAME_RAW)

# Special variable for version.h script only
LIBNAME_FILES = halcs_client

# Config variables suitable for creating shared libraries
LIB_VER = $(shell ./version.sh $(LIBNAME_FILES))
LIB_VER_MAJOR = $(shell echo $(LIB_VER)| cut -d'.' -f1)
LIB_VER_MINOR = $(shell echo $(LIB_VER)| cut -d'.' -f2)
LIB_VER_REVESION = $(shell echo $(LIB_VER)| cut -d'.' -f3)

PREFIX ?= /usr/local
export PREFIX

# General C/CPP flags
CFLAGS_USR = -std=gnu99 -O2 -fPIC
# We expect tghese variables to be appended to the possible
# command-line options
override CPPFLAGS +=
override CXXFLAGS +=

# Malamute 1.0.0 requires this to be defined
# as all of its API is in DRAFT state
CFLAGS_USR += -DMLM_BUILD_DRAFT_API

CFLAGS_DEBUG =

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
LIBS =

# General library flags -L<libdir>
LFLAGS =

# Source directory
SRC_DIR = src
# Include directory
INCLUDE_DIR = include

SMIO_DIR = ../sm_io
SMIO_SRC_DIR = $(SMIO_DIR)

SMIO_TABLE_DIR = ../sm_io_table
SMIO_TABLE_SRC_DIR = $(SMIO_TABLE_DIR)

# Specific board objects
OBJS_BOARD = ../boards/afcv3_1/acq_chan.o \
             ../boards/afcv3/acq_chan.o \
             ../boards/ml605/acq_chan.o
# Specific platform objects
OBJS_PLATFORM =
# Objects that are not specificaly related to the halcs_client, but composes
# our library, as it eases some tasks for us
OBJS_EXTERNAL = $(SMIO_TABLE_SRC_DIR)/sm_io_codes.o \
                $(SMIO_TABLE_SRC_DIR)/modules/acq/sm_io_acq_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/dsp/sm_io_dsp_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/fmc130m_4ch/sm_io_fmc130m_4ch_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/fmc250m_4ch/sm_io_fmc250m_4ch_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/fmc100m_4ch/sm_io_fmc100m_4ch_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/fmcpico1m_4ch/sm_io_fmcpico1m_4ch_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/fmc_adc_common/sm_io_fmc_adc_common_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/fmc_active_clk/sm_io_fmc_active_clk_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/swap/sm_io_swap_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/rffe/sm_io_rffe_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/rffe_scpi/sm_io_rffe_scpi_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/afc_diag/sm_io_afc_diag_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/trigger_iface/sm_io_trigger_iface_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/trigger_mux/sm_io_trigger_mux_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/afc_timing/sm_io_afc_timing_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/acq_pm/sm_io_acq_pm_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/init/sm_io_init_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/afc_mgmt/sm_io_afc_mgmt_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/tim_rcv/sm_io_tim_rcv_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/orbit_intlk/sm_io_orbit_intlk_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/fofb_ctrl/sm_io_fofb_ctrl_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/rtmlamp_ohwr/sm_io_rtmlamp_ohwr_exports.o \
                $(SMIO_TABLE_SRC_DIR)/modules/fofb_processing/sm_io_fofb_processing_exports.o

# Include directories
INCLUDE_DIRS = -I. -Iinclude \
	       -I$(TOP)/include \
	       -I$(SMIO_DIR) \
	       -I$(SMIO_SRC_DIR)/modules \
	       -I$(SMIO_SRC_DIR)/modules/fmc130m_4ch \
	       -I$(SMIO_SRC_DIR)/modules/fmc250m_4ch \
	       -I$(SMIO_SRC_DIR)/modules/fmc100m_4ch \
	       -I$(SMIO_SRC_DIR)/modules/fmcpico1m_4ch \
	       -I$(SMIO_SRC_DIR)/modules/fmc_adc_common \
	       -I$(SMIO_SRC_DIR)/modules/fmc_active_clk \
	       -I$(SMIO_SRC_DIR)/modules/acq \
	       -I$(SMIO_SRC_DIR)/modules/swap \
	       -I$(SMIO_SRC_DIR)/modules/dsp \
	       -I$(SMIO_SRC_DIR)/modules/rffe \
	       -I$(SMIO_SRC_DIR)/modules/rffe_scpi \
	       -I$(SMIO_SRC_DIR)/modules/afc_diag \
	       -I$(SMIO_SRC_DIR)/modules/trigger_iface \
	       -I$(SMIO_SRC_DIR)/modules/trigger_mux \
	       -I$(SMIO_SRC_DIR)/modules/afc_timing \
	       -I$(SMIO_SRC_DIR)/modules/acq_pm \
	       -I$(SMIO_SRC_DIR)/modules/init \
	       -I$(SMIO_SRC_DIR)/modules/afc_mgmt \
	       -I$(SMIO_SRC_DIR)/modules/tim_rcv \
	       -I$(SMIO_SRC_DIR)/modules/orbit_intlk \
	       -I$(SMIO_SRC_DIR)/modules/fofb_ctrl \
	       -I$(SMIO_SRC_DIR)/modules/rtmlamp_ohwr \
               -I$(SMIO_SRC_DIR)/modules/fofb_processing \
	       -I$(SMIO_TABLE_DIR) \
	       -I$(SMIO_TABLE_SRC_DIR)/modules \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/fmc130m_4ch \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/fmc250m_4ch \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/fmc100m_4ch \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/fmcpico1m_4ch \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/fmc_adc_common \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/fmc_active_clk \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/acq \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/swap \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/dsp \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/rffe \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/rffe_scpi \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/afc_diag \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/trigger_iface \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/trigger_mux \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/afc_timing \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/acq_pm \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/init \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/afc_mgmt \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/tim_rcv \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/orbit_intlk \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/fofb_ctrl \
	       -I$(SMIO_TABLE_SRC_DIR)/modules/rtmlamp_ohwr \
               -I$(SMIO_TABLE_SRC_DIR)/modules/fofb_processing \
	       -I${PREFIX}/include \
	       -I$(LIBERRHAND_DIR)/include \
	       -I$(LIBHUTILS_DIR)/include \
	       -I$(LIBDISPTABLE_DIR)/include

# Merge all flags. We expect tghese variables to be appended to the possible
# command-line options
override CFLAGS += $(CFLAGS_USR) $(CFLAGS_PLATFORM) $(CFLAGS_DEBUG) $(CPPFLAGS) $(CXXFLAGS)
override LDFLAGS += $(LFLAGS) $(LDFLAGS_PLATFORM)

# Output library names
OUT = $(LIBNAME)

.SECONDEXPANSION:

# Library objects
$(LIBNAME)_OBJS_LIB = $(SRC_DIR)/halcs_client_core.o $(SRC_DIR)/halcs_client_err.o \
	$(SRC_DIR)/halcs_client_rw_param.o

# Objects common for both server and client libraries.
common_OBJS = $(OBJS_BOARD) $(OBJS_PLATFORM) $(OBJS_EXTERNAL)

# Objects for each version of library
$(LIBNAME)_OBJS = $(common_OBJS) $($(LIBNAME)_OBJS_LIB)
$(LIBNAME)_CODE_HEADERS = \
	$(TOP)/include/acq_chan_gen_defs.h

$(LIBNAME)_SMIO_CODES = \
	$(SMIO_SRC_DIR)/modules/fmc130m_4ch/sm_io_fmc130m_4ch_codes.h \
	$(SMIO_SRC_DIR)/modules/fmc250m_4ch/sm_io_fmc250m_4ch_codes.h \
	$(SMIO_SRC_DIR)/modules/fmc100m_4ch/sm_io_fmc100m_4ch_codes.h \
	$(SMIO_SRC_DIR)/modules/fmcpico1m_4ch/sm_io_fmcpico1m_4ch_codes.h \
	$(SMIO_SRC_DIR)/modules/fmc_adc_common/sm_io_fmc_adc_common_codes.h \
	$(SMIO_SRC_DIR)/modules/fmc_active_clk/sm_io_fmc_active_clk_codes.h \
	$(SMIO_SRC_DIR)/modules/acq/sm_io_acq_codes.h \
	$(SMIO_SRC_DIR)/modules/dsp/sm_io_dsp_codes.h \
	$(SMIO_SRC_DIR)/modules/swap/sm_io_swap_codes.h \
	$(SMIO_SRC_DIR)/modules/rffe/sm_io_rffe_codes.h \
	$(SMIO_SRC_DIR)/modules/afc_diag/sm_io_afc_diag_codes.h \
	$(SMIO_SRC_DIR)/modules/trigger_iface/sm_io_trigger_iface_codes.h \
	$(SMIO_SRC_DIR)/modules/trigger_mux/sm_io_trigger_mux_codes.h \
	$(SMIO_SRC_DIR)/modules/afc_timing/sm_io_afc_timing_codes.h \
	$(SMIO_SRC_DIR)/modules/acq_pm/sm_io_acq_pm_codes.h \
	$(SMIO_SRC_DIR)/modules/init/sm_io_init_codes.h \
	$(SMIO_SRC_DIR)/modules/afc_mgmt/sm_io_afc_mgmt_codes.h \
	$(SMIO_SRC_DIR)/modules/tim_rcv/sm_io_tim_rcv_codes.h \
	$(SMIO_SRC_DIR)/modules/orbit_intlk/sm_io_orbit_intlk_codes.h \
	$(SMIO_SRC_DIR)/modules/fofb_ctrl/sm_io_fofb_ctrl_codes.h \
	$(SMIO_SRC_DIR)/modules/rtmlamp_ohwr/sm_io_rtmlamp_ohwr_codes.h \
        $(SMIO_SRC_DIR)/modules/fofb_processing/sm_io_fofb_processing_codes.h \
	$(SMIO_TABLE_DIR)/sm_io_codes.h

$(LIBNAME)_SMIO_EXPORTS = \
	$(SMIO_TABLE_SRC_DIR)/modules/fmc130m_4ch/sm_io_fmc130m_4ch_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/fmc250m_4ch/sm_io_fmc250m_4ch_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/fmc100m_4ch/sm_io_fmc100m_4ch_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/fmcpico1m_4ch/sm_io_fmcpico1m_4ch_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/fmc_adc_common/sm_io_fmc_adc_common_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/fmc_active_clk/sm_io_fmc_active_clk_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/acq/sm_io_acq_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/dsp/sm_io_dsp_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/swap/sm_io_swap_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/rffe/sm_io_rffe_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/afc_diag/sm_io_afc_diag_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/trigger_iface/sm_io_trigger_iface_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/trigger_mux/sm_io_trigger_mux_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/afc_timing/sm_io_afc_timing_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/acq_pm/sm_io_acq_pm_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/init/sm_io_init_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/afc_mgmt/sm_io_afc_mgmt_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/tim_rcv/sm_io_tim_rcv_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/orbit_intlk/sm_io_orbit_intlk_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/fofb_ctrl/sm_io_fofb_ctrl_exports.h \
	$(SMIO_TABLE_SRC_DIR)/modules/rtmlamp_ohwr/sm_io_rtmlamp_ohwr_exports.h \
        $(SMIO_TABLE_SRC_DIR)/modules/fofb_processing/sm_io_fofb_processing_exports.h 

# Copy non-standard headers
$(LIBNAME)_SMIO_HEADERS_BASENAME = $(basename $(notdir $($(LIBNAME)_SMIO_CODES) $($(LIBNAME)_SMIO_EXPORTS)))
$(LIBNAME)_SMIO_HEADERS = $($(LIBNAME)_SMIO_CODES) $($(LIBNAME)_SMIO_EXPORTS)
$(LIBNAME)_INST_SMIO_HEADERS = $(addprefix include/,$(addsuffix .h,$($(LIBNAME)_SMIO_HEADERS_BASENAME)))

# Our local library headers
$(LIBNAME)_HEADERS_LIB = $(INCLUDE_DIR)/halcs_client.h $(INCLUDE_DIR)/halcs_client_classes.h \
	$(INCLUDE_DIR)/halcs_client_prelude.h $(INCLUDE_DIR)/halcs_client_codes.h \
	$(subst $(SRC_DIR)/,$(INCLUDE_DIR)/, $(patsubst %.o,%.h,$($(LIBNAME)_OBJS_LIB)))

$(LIBNAME)_HEADERS = $($(LIBNAME)_HEADERS_LIB) $($(LIBNAME)_CODE_HEADERS) \
	$($(LIBNAME)_INST_SMIO_HEADERS)

# Installation headers
$(LIBNAME)_INST_HEADERS = $($(LIBNAME)_HEADERS)

# Save a git repository description
GIT_REVISION = $(shell git describe --dirty --always)
GIT_USER_NAME = $(shell git config --get user.name)
GIT_USER_EMAIL = $(shell git config --get user.email)

revision_OBJS = $(SRC_DIR)/halcs_client_revision.o
revision_SRCS = $(patsubst %.o,%.c,$(revision_OBJS))

OBJS_all = $(common_OBJS) $($(LIBNAME)_OBJS) \
	   $(revision_OBJS)

# Libraries suffixes
LIB_STATIC_SUFFIX = .a
LIB_SHARED_SUFFIX = .so

# Generate suitable names for static libraries
TARGET_STATIC = $(addsuffix $(LIB_STATIC_SUFFIX), $(OUT))
TARGET_SHARED = $(addsuffix $(LIB_SHARED_SUFFIX), $(OUT))
TARGET_SHARED_VER = $(addsuffix $(LIB_SHARED_SUFFIX).$(LIB_VER), $(OUT))

.PHONY: all clean mrproper pre_inst pre_clean install uninstall

# Avoid deletion of intermediate files, such as objects
.SECONDARY: $(OBJS_all)

# Makefile rules
all: $(TARGET_STATIC) $(TARGET_SHARED_VER) pre_inst

# Compile static library
%.a: $$($$*_OBJS) $(revision_OBJS)
	$(AR) rcs $@ $^

# Compile dynamic library
%.so.$(LIB_VER): $$($$*_OBJS) $(revision_OBJS)
	$(CC) $(LDFLAGS) -shared -fPIC -Wl,-soname,$@ -o $@ $^ $(LIBS)

# Special rule for the revision object
$(revision_OBJS): $(revision_SRCS)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -DGIT_REVISION="\"$(GIT_REVISION)\"" \
        -DGIT_USER_NAME="\"$(GIT_USER_NAME)\"" \
        -DGIT_USER_EMAIL="\"$(GIT_USER_EMAIL)\"" \
        -c $< -o $@

# Pull in dependency info for *existing* .o files and don't complain if the
# corresponding .d file is not found
-include $(OBJS_all:.o=.d)

# Compile with position-independent objects.
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

#tests:
#	$(MAKE) -C $@ all
#

# Prepare the environment for install rule
pre_inst:
	$(foreach header,$($(LIBNAME)_SMIO_HEADERS),cp -p $(header) \
		$(PWD)/include/ $(CMDSEP))

pre_clean:
	$(foreach header,$(PWD)/$($(LIBNAME)_INST_SMIO_HEADERS), \
		rm -f $(header) $(CMDSEP))

install: all
	$(foreach lib,$(TARGET_SHARED_VER),install -m 644 $(lib) $(PREFIX)/lib $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),ln -sf $(lib).$(LIB_VER) $(PREFIX)/lib/$(lib) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),ln -sf $(lib).$(LIB_VER) $(PREFIX)/lib/$(lib).$(LIB_VER_MAJOR) $(CMDSEP))
	$(foreach lib,$(TARGET_STATIC),install -m 644 $(lib) $(PREFIX)/lib $(CMDSEP))
	$(foreach header,$($(LIBNAME)_INST_HEADERS),install -m 644 $(header) $(PREFIX)/include $(CMDSEP))

uninstall:
	$(foreach lib,$(TARGET_SHARED),rm -f $(PREFIX)/lib/$(lib).$(LIB_VER) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),rm -f $(PREFIX)/lib/$(lib) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),rm -f $(PREFIX)/lib/$(lib).$(LIB_VER_MAJOR) $(CMDSEP))
	$(foreach lib,$(TARGET_STATIC),rm -f $(PREFIX)/lib/$(lib) $(CMDSEP))
	$(foreach header,$(notdir $($(LIBNAME)_INST_HEADERS)),rm -f \
		$(PREFIX)/include/$(header) $(CMDSEP))

clean: pre_clean
	rm -f $(OBJS_all) $(OBJS_all:.o=.d) $(REVISION_NAME).o

mrproper: clean
	rm -f *.a *.so.$(LIB_VER)

