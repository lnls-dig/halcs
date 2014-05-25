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

INSTALL_DIR ?= /usr/local/lib
export INSTALL_DIR

# Config variables suitable for creating shared libraries
LIB_VER = 1.0.0

# General C flags
CFLAGS = -std=gnu99 -O2

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

# Specific platform objects
OBJS_PLATFORM =
# Library objects
libbpmclient_OBJS_LIB = bpm_client.o bpm_client_err.o

# Include directories
INCLUDE_DIRS = -I. -I../hal/include -I../hal/debug -I../hal/sm_io/modules/ \
	       -I../hal/sm_io/modules/fmc130m_4ch -I../hal/sm_io/modules/acq

# Merge all flags. Optimize for size (-Os)
CFLAGS += $(CFLAGS_PLATFORM) $(CFLAGS_DEBUG)

LDFLAGS = $(LDFLAGS_PLATFORM)

# Output library names
LIBCLIENT = libbpmclient
OUT = $(LIBCLIENT)

.SECONDEXPANSION:

# Objects common for both server and client libraries
common_OBJS = $(OBJS_PLATFORM) $(OBJS_BOARD)

# Objects for each version of library
$(LIBCLIENT)_OBJS = $(common_OBJS) $($(LIBCLIENT)_OBJS_LIB)
$(LIBCLIENT)_CODE_HEADERS = ../hal/sm_io/modules/fmc130m_4ch/sm_io_fmc130m_4ch_codes.h \
	../hal/sm_io/modules/acq/sm_io_acq_codes.h
$(LIBCLIENT)_HEADERS = $($(LIBCLIENT)_OBJS_LIB:.o=.h) $($(LIBCLIENT)_CODE_HEADERS)

# Save a git repository description
REVISION = $(shell git describe --dirty --always)
REVISION_NAME = revision
OBJ_REVISION = $(addsuffix .o, $(REVISION_NAME))

OBJS_all = $(common_OBJS) $($(LIBCLIENT)_OBJS_LIB) \
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
all: $(TARGET_STATIC) $(TARGET_SHARED_VER)

# Compile static library
%.a: $$($$*_OBJS) $(OBJ_REVISION)
	$(AR) rcs $@ $?

# Compile dynamic library
%.so.$(LIB_VER): $$($$*_OBJS) $(OBJ_REVISION)
	$(CC) -shared -fPIC -Wl,-soname,$@ -o $@ $? $(LDFLAGS)

$(REVISION_NAME).o: $(REVISION_NAME).c
	$(CC) $(CFLAGS) -DGIT_REVISION=\"$(REVISION)\" -c $<

# Pull in dependency info for *existing* .o files and don't complain if the
# corresponding .d file is not found
-include $(OBJS_all:.o=.d)

# Compile with position-independent objects.
# Autodependencies generatation by Scott McPeak, November 2001,
# from article "Autodependencies with GNU make"
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c -fPIC $*.c -o $@

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

install:
	$(foreach lib,$(TARGET_SHARED_VER),install -m 755 $(lib) $(INSTALL_DIR) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),ln -sf $(lib).$(LIB_VER) $(INSTALL_DIR)/$(lib) $(CMDSEP))
	$(foreach lib,$(TARGET_STATIC),install -m 755 $(lib) $(INSTALL_DIR) $(CMDSEP))
	$(foreach header,$($(LIBCLIENT)_HEADERS),install -m 755 $(header) $(INSTALL_DIR) $(CMDSEP))

uninstall:
	$(foreach lib,$(TARGET_SHARED),rm -f $(INSTALL_DIR)/$(lib).$(LIB_VER) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),rm -f $(INSTALL_DIR)/$(lib) $(CMDSEP))
	$(foreach lib,$(TARGET_STATIC),rm -f $(INSTALL_DIR)/$(lib) $(CMDSEP))
	$(foreach header,$(notdir $($(LIBCLIENT)_HEADERS)),rm -f $(INSTALL_DIR)/$(header) $(CMDSEP))

clean:
	rm -f $(OBJS_all) $(OBJS_all:.o=.d) $(REVISION_NAME).o

mrproper: clean
	rm -f *.a *.so.$(LIB_VER)

