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

PWD = 		$(shell pwd)

# Select board in which we will work. Options are: ml605 or afcv3
BOARD ?= ml605

INSTALL_DIR ?= /usr/local
export INSTALL_DIR

LIBCLIENT = libbpmclient

# Config variables suitable for creating shared libraries
LIB_VER_MAJOR = 1
LIB_VER_MINOR = 0
LIB_VER_REVISION = 0
LIB_VER = $(LIB_VER_MAJOR).$(LIB_VER_MINOR).$(LIB_VER_REVISION)

# General C flags
CFLAGS = -std=gnu99 -O2 -fPIC

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
LIBS = -lzmq -lczmq -lmdp

# General library flags -L<libdir>
LFLAGS =

# Specific board objects
OBJS_BOARD = ../hal/sm_io/modules/acq/ddr3_map.o
# Specific platform objects
OBJS_PLATFORM = ../hal/debug/debug_print.o ../hal/debug/local_print.o \
		../hal/debug/debug_subsys.o
# Objects that are not specificaly related to the bpm_client, but composes
# our library, as it eases some tasks for us
OBJS_EXTERNAL = ../hal/sm_io/modules/sm_io_codes.o \
                ../hal/sm_io/modules/acq/sm_io_acq_exports.o \
                ../hal/sm_io/modules/dsp/sm_io_dsp_exports.o \
                ../hal/sm_io/modules/fmc130m_4ch/sm_io_fmc130m_4ch_exports.o \
                ../hal/sm_io/modules/swap/sm_io_swap_exports.o \
                ../hal/sm_io/modules/rffe/sm_io_rffe_exports.o \
                ../hal/sm_io/modules/afc_diag/sm_io_afc_diag_exports.o

# Include directories
INCLUDE_DIRS = -I. -I../hal/include -I../hal/debug \
	       -I../hal/sm_io/modules \
	       -I../hal/sm_io/modules/fmc130m_4ch \
	       -I../hal/sm_io/modules/acq \
	       -I../hal/sm_io/modules/swap \
	       -I../hal/sm_io/modules/dsp \
	       -I../hal/sm_io/modules/rffe \
	       -I../hal/sm_io/modules/afc_diag \
	       -I../hal/sm_io/rw_param \
	       -I../hal/hal_utils \
	       -I../hal/include/hw \
	       -I../hal/boards/$(BOARD) \
	       -I/usr/local/include

# Merge all flags. Optimize for size (-Os)
CFLAGS += $(CFLAGS_PLATFORM) $(CFLAGS_DEBUG)

LDFLAGS = $(LDFLAGS_PLATFORM)

# Output library names
OUT = $(LIBCLIENT)

.SECONDEXPANSION:

# Library objects
$(LIBCLIENT)_OBJS_LIB = bpm_client.o bpm_client_err.o rw_param_client.o

# Objects common for both server and client libraries.
common_OBJS = $(OBJS_BOARD) $(OBJS_PLATFORM) $(OBJS_EXTERNAL)

# Objects for each version of library
$(LIBCLIENT)_OBJS = $(common_OBJS) $($(LIBCLIENT)_OBJS_LIB)
$(LIBCLIENT)_CODE_HEADERS = ../hal/sm_io/modules/fmc130m_4ch/sm_io_fmc130m_4ch_codes.h \
	../hal/sm_io/modules/acq/sm_io_acq_codes.h \
	../hal/sm_io/modules/dsp/sm_io_dsp_codes.h \
	../hal/sm_io/modules/swap/sm_io_swap_codes.h \
	../hal/sm_io/modules/rffe/sm_io_rffe_codes.h \
	../hal/sm_io/modules/afc_diag/sm_io_afc_diag_codes.h \
	../hal/sm_io/modules/sm_io_codes.h \
	../hal/include/acq_chan_gen_defs.h \
	../hal/hal_utils/dispatch_table.h \
	../hal/hal_utils/hal_utils_err.h \
	bpm_client_codes.h

$(LIBCLIENT)_FUNC_EXPORTS = ../hal/sm_io/modules/fmc130m_4ch/sm_io_fmc130m_4ch_exports.h \
	../hal/sm_io/modules/acq/sm_io_acq_exports.h \
	../hal/sm_io/modules/dsp/sm_io_dsp_exports.h \
	../hal/sm_io/modules/swap/sm_io_swap_exports.h \
	../hal/sm_io/modules/rffe/sm_io_rffe_exports.h \
	../hal/sm_io/modules/afc_diag/sm_io_afc_diag_exports.h

# Install only specific acq_chan.h defintions according to the BOARD MACRO
#	../hal/include/mem_layout/ml605/acq_chan_ml605.h \
#	../hal/include/mem_layout/afcv3/acq_chan_afcv3.h

$(LIBCLIENT)_ACQ_HEADER_BASENAME = acq_chan
$(LIBCLIENT)_ACQ_HEADERS += \
	../hal/include/board/$(BOARD)/$($(LIBCLIENT)_ACQ_HEADER_BASENAME)_$(BOARD).h

$(LIBCLIENT)_HEADERS = $($(LIBCLIENT)_OBJS_LIB:.o=.h) $($(LIBCLIENT)_CODE_HEADERS) \
    $($(LIBCLIENT)_FUNC_EXPORTS)

# Installation headers
$(LIBCLIENT)_INST_HEADERS = $($(LIBCLIENT)_HEADERS) $($(LIBCLIENT)_ACQ_HEADER_BASENAME).h

# Save a git repository description
REVISION = $(shell git describe --dirty --always)
REVISION_NAME = revision
OBJ_REVISION = $(addsuffix .o, $(REVISION_NAME))

OBJS_all = $(common_OBJS) $($(LIBCLIENT)_OBJS) \
	   $(OBJ_REVISION)

# Libraries suffixes
LIB_STATIC_SUFFIX = .a
LIB_SHARED_SUFFIX = .so

# Generate suitable names for static libraries
TARGET_STATIC = $(addsuffix $(LIB_STATIC_SUFFIX), $(OUT))
TARGET_SHARED = $(addsuffix $(LIB_SHARED_SUFFIX), $(OUT))
TARGET_SHARED_VER = $(addsuffix $(LIB_SHARED_SUFFIX).$(LIB_VER), $(OUT))

.PHONY: all clean mrproper install uninstall

# Avoid deletion of intermediate files, such as objects
.SECONDARY: $(OBJS_all)

# Makefile rules
all: $(TARGET_STATIC) $(TARGET_SHARED_VER) pre_inst

# Compile static library
%.a: $$($$*_OBJS) $(OBJ_REVISION)
	$(AR) rcs $@ $^

# Compile dynamic library
%.so.$(LIB_VER): $$($$*_OBJS) $(OBJ_REVISION)
	$(CC) -shared -fPIC -Wl,-soname,$@ -o $@ $^ $(LDFLAGS)

$(REVISION_NAME).o: $(REVISION_NAME).c
	$(CC) $(CFLAGS) -DGIT_REVISION=\"$(REVISION)\" -c $<

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
	$(foreach header,$($(LIBCLIENT)_ACQ_HEADERS),cp -p $(header) \
		$(PWD)/$($(LIBCLIENT)_ACQ_HEADER_BASENAME).h $(CMDSEP))

pre_clean:
	$(foreach header,$(PWD)/$($(LIBCLIENT)_ACQ_HEADER_BASENAME).h, \
		rm -f $(header) $(CMDSEP))

install:
	$(foreach lib,$(TARGET_SHARED_VER),install -m 755 $(lib) $(INSTALL_DIR)/lib $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),ln -sf $(lib).$(LIB_VER) $(INSTALL_DIR)/lib/$(lib) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),ln -sf $(lib).$(LIB_VER) $(INSTALL_DIR)/lib/$(lib).$(LIB_VER_MAJOR) $(CMDSEP))
	$(foreach lib,$(TARGET_STATIC),install -m 755 $(lib) $(INSTALL_DIR)/lib $(CMDSEP))
	$(foreach header,$($(LIBCLIENT)_INST_HEADERS),install -m 755 $(header) $(INSTALL_DIR)/include $(CMDSEP))

uninstall:
	$(foreach lib,$(TARGET_SHARED),rm -f $(INSTALL_DIR)/lib/$(lib).$(LIB_VER) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),rm -f $(INSTALL_DIR)/lib/$(lib) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),rm -f $(INSTALL_DIR)/lib/$(lib).$(LIB_VER_MAJOR) $(CMDSEP))
	$(foreach lib,$(TARGET_STATIC),rm -f $(INSTALL_DIR)/lib/$(lib) $(CMDSEP))
	$(foreach header,$(notdir $($(LIBCLIENT)_INST_HEADERS)),rm -f \
		$(INSTALL_DIR)/include/$(header) $(CMDSEP))

clean: pre_clean
	rm -f $(OBJS_all) $(OBJS_all:.o=.d) $(REVISION_NAME).o

mrproper: clean
	rm -f *.a *.so.$(LIB_VER)

