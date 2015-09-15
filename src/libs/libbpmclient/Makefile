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

PWD = 		$(shell pwd)

# Select board in which we will work. Options are: ml605 or afcv3
BOARD ?= ml605

PREFIX ?= /usr/local
export PREFIX

LIBBPMCLIENT = libbpmclient

# Config variables suitable for creating shared libraries
LIB_VER = $(shell ./version.sh bpm_client)

# General C/CPP flags
CFLAGS_USR = -std=gnu99 -O2 -fPIC
# We expect tghese variables to be appended to the possible
# command-line options
override CPPFLAGS +=
override CXXFLAGS +=

ifeq ($(BOARD),ml605)
CFLAGS_USR += -D__BOARD_ML605__
endif

ifeq ($(BOARD),afcv3)
CFLAGS_USR += -D__BOARD_AFCV3__
endif

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
LIBS = -lzmq -lczmq -lmlm -luuid

# General library flags -L<libdir>
LFLAGS =

# Source directory
SRC_DIR = src

# Specific board objects
OBJS_BOARD =
# Specific platform objects
OBJS_PLATFORM =
# Objects that are not specificaly related to the bpm_client, but composes
# our library, as it eases some tasks for us
OBJS_EXTERNAL = ../../sm_io/modules/sm_io_codes.o \
                ../../sm_io/modules/acq/sm_io_acq_exports.o \
                ../../sm_io/modules/dsp/sm_io_dsp_exports.o \
                ../../sm_io/modules/fmc130m_4ch/sm_io_fmc130m_4ch_exports.o \
                ../../sm_io/modules/swap/sm_io_swap_exports.o \
                ../../sm_io/modules/rffe/sm_io_rffe_exports.o \
                ../../sm_io/modules/afc_diag/sm_io_afc_diag_exports.o

# Project boards
boards_INCLUDE_DIRS = -I../../../include/boards/$(BOARD)

# Include directories
INCLUDE_DIRS = -I. -Iinclude -I../liberrhand -I../libhutils -I../libdisptable \
		   -I../libconvc \
	       $(boards_INCLUDE_DIRS) \
	       -I../../../include \
	       -I../../sm_io/modules \
	       -I../../sm_io/modules/fmc130m_4ch \
	       -I../../sm_io/modules/acq \
	       -I../../sm_io/modules/swap \
	       -I../../sm_io/modules/dsp \
	       -I../../sm_io/modules/rffe \
	       -I../../sm_io/modules/afc_diag \
	       -I/usr/local/include

# Merge all flags. We expect tghese variables to be appended to the possible
# command-line options
override CFLAGS += $(CFLAGS_USR) $(CFLAGS_PLATFORM) $(CFLAGS_DEBUG) $(CPPFLAGS) $(CXXFLAGS)
override LDFLAGS += $(LFLAGS) $(LDFLAGS_PLATFORM)

# Output library names
OUT = $(LIBBPMCLIENT)

.SECONDEXPANSION:

# Library objects
$(LIBBPMCLIENT)_OBJS_LIB = $(SRC_DIR)/bpm_client_core.o $(SRC_DIR)/bpm_client_err.o \
	$(SRC_DIR)/bpm_client_rw_param.o

# Objects common for both server and client libraries.
common_OBJS = $(OBJS_BOARD) $(OBJS_PLATFORM) $(OBJS_EXTERNAL)

# Objects for each version of library
$(LIBBPMCLIENT)_OBJS = $(common_OBJS) $($(LIBBPMCLIENT)_OBJS_LIB)
$(LIBBPMCLIENT)_CODE_HEADERS = \
	../../../include/acq_chan_gen_defs.h \
	../libdisptable/disp_table.h \
	../libdisptable/disp_table_err.h \
	../libhutils/hutils.h \
	../libhutils/hutils_err.h \
	../libhutils/hutils_math.h \
	../libhutils/hutils_utils.h

$(LIBBPMCLIENT)_SMIO_CODES = ../../sm_io/modules/fmc130m_4ch/sm_io_fmc130m_4ch_codes.h \
	../../sm_io/modules/acq/sm_io_acq_codes.h \
	../../sm_io/modules/dsp/sm_io_dsp_codes.h \
	../../sm_io/modules/swap/sm_io_swap_codes.h \
	../../sm_io/modules/rffe/sm_io_rffe_codes.h \
	../../sm_io/modules/afc_diag/sm_io_afc_diag_codes.h \
	../../sm_io/modules/sm_io_codes.h

$(LIBBPMCLIENT)_SMIO_EXPORTS = ../../sm_io/modules/fmc130m_4ch/sm_io_fmc130m_4ch_exports.h \
	../../sm_io/modules/acq/sm_io_acq_exports.h \
	../../sm_io/modules/dsp/sm_io_dsp_exports.h \
	../../sm_io/modules/swap/sm_io_swap_exports.h \
	../../sm_io/modules/rffe/sm_io_rffe_exports.h \
	../../sm_io/modules/afc_diag/sm_io_afc_diag_exports.h

# Copy specific acq_chan.h defintions according to the BOARD MACRO
$(LIBBPMCLIENT)_ACQ_HEADERS_BASENAME = acq_chan
$(LIBBPMCLIENT)_ACQ_HEADERS = \
	../../../include/boards/$(BOARD)/priv_defs/$($(LIBBPMCLIENT)_ACQ_HEADERS_BASENAME)_$(BOARD).h
$(LIBBPMCLIENT)_INST_ACQ_HEADERS = include/$($(LIBBPMCLIENT)_ACQ_HEADERS_BASENAME).h

# Copy non-standard headers
$(LIBBPMCLIENT)_SMIO_HEADERS_BASENAME = $(basename $(notdir $($(LIBBPMCLIENT)_SMIO_CODES) $($(LIBBPMCLIENT)_SMIO_EXPORTS)))
$(LIBBPMCLIENT)_SMIO_HEADERS = $($(LIBBPMCLIENT)_SMIO_CODES) $($(LIBBPMCLIENT)_SMIO_EXPORTS)
$(LIBBPMCLIENT)_INST_SMIO_HEADERS = $(addprefix include/,$(addsuffix .h,$($(LIBBPMCLIENT)_SMIO_HEADERS_BASENAME)))

# Our local library headers
$(LIBBPMCLIENT)_HEADERS_LIB = include/bpm_client.h include/bpm_client_classes.h \
	include/bpm_client_prelude.h include/bpm_client_codes.h \
	$(subst src/,include/, $(patsubst %.o,%.h,$($(LIBBPMCLIENT)_OBJS_LIB)))

$(LIBBPMCLIENT)_HEADERS = $($(LIBBPMCLIENT)_HEADERS_LIB) $($(LIBBPMCLIENT)_CODE_HEADERS) \
	$($(LIBBPMCLIENT)_INST_SMIO_HEADERS)

# Installation headers
$(LIBBPMCLIENT)_INST_HEADERS = $($(LIBBPMCLIENT)_HEADERS) $($(LIBBPMCLIENT)_INST_ACQ_HEADERS)

# Save a git repository description
GIT_REVISION = $(shell git describe --dirty --always)
GIT_USER_NAME = $(shell git config --get user.name)
GIT_USER_EMAIL = $(shell git config --get user.email)

revision_OBJS = $(SRC_DIR)/bpm_client_revision.o
revision_SRCS = $(patsubst %.o,%.c,$(revision_OBJS))

OBJS_all = $(common_OBJS) $($(LIBBPMCLIENT)_OBJS) \
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
	$(CC) -shared -fPIC -Wl,-soname,$@ -o $@ $^ $(LDFLAGS) $(LIBS)

# Special rule for the revision object
$(revision_OBJS): $(revision_SRCS)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -DGIT_REVISION="\"$(GIT_REVISION)\"" \
        -DGIT_USER_NAME="\"$(GIT_USER_NAME)\"" \
        -DGIT_USER_EMAIL="\"$(GIT_USER_EMAIL)\"" \
        -c $< -o $@ $(LDFLAGS)

# Pull in dependency info for *existing* .o files and don't complain if the
# corresponding .d file is not found
-include $(OBJS_all:.o=.d)

# Compile with position-independent objects.
# Autodependencies generatation by Scott McPeak, November 2001,
# from article "Autodependencies with GNU make"
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $*.c -o $@ $(LDFLAGS)

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
	$(foreach header,$($(LIBBPMCLIENT)_ACQ_HEADERS),cp -p $(header) \
		$(PWD)/$($(LIBBPMCLIENT)_INST_ACQ_HEADERS) $(CMDSEP))
	$(foreach header,$($(LIBBPMCLIENT)_SMIO_HEADERS),cp -p $(header) \
		$(PWD)/include/ $(CMDSEP))

pre_clean:
	$(foreach header,$(PWD)/$($(LIBBPMCLIENT)_INST_ACQ_HEADERS), \
		rm -f $(header) $(CMDSEP))
	$(foreach header,$(PWD)/$($(LIBBPMCLIENT)_INST_SMIO_HEADERS), \
		rm -f $(header) $(CMDSEP))

install:
	$(foreach lib,$(TARGET_SHARED_VER),install -m 755 $(lib) $(PREFIX)/lib $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),ln -sf $(lib).$(LIB_VER) $(PREFIX)/lib/$(lib) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),ln -sf $(lib).$(LIB_VER) $(PREFIX)/lib/$(lib).$(LIB_VER_MAJOR) $(CMDSEP))
	$(foreach lib,$(TARGET_STATIC),install -m 755 $(lib) $(PREFIX)/lib $(CMDSEP))
	$(foreach header,$($(LIBBPMCLIENT)_INST_HEADERS),install -m 755 $(header) $(PREFIX)/include $(CMDSEP))

uninstall:
	$(foreach lib,$(TARGET_SHARED),rm -f $(PREFIX)/lib/$(lib).$(LIB_VER) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),rm -f $(PREFIX)/lib/$(lib) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),rm -f $(PREFIX)/lib/$(lib).$(LIB_VER_MAJOR) $(CMDSEP))
	$(foreach lib,$(TARGET_STATIC),rm -f $(PREFIX)/lib/$(lib) $(CMDSEP))
	$(foreach header,$(notdir $($(LIBBPMCLIENT)_INST_HEADERS)),rm -f \
		$(PREFIX)/include/$(header) $(CMDSEP))

clean: pre_clean
	rm -f $(OBJS_all) $(OBJS_all:.o=.d) $(REVISION_NAME).o

mrproper: clean
	rm -f *.a *.so.$(LIB_VER)

