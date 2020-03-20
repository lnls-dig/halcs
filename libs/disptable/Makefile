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

LIBNAME_RAW = disptable
LIBNAME = lib$(LIBNAME_RAW)

# Config variables suitable for creating shared libraries
LIB_VER = $(shell ./version.sh $(LIBNAME_RAW))
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

# Include directories
INCLUDE_DIRS = -Iinclude \
	           -I${PREFIX}/include \
	           -I$(LIBERRHAND_DIR)/include \
	           -I$(LIBHUTILS_DIR)/include

# Merge all flags. We expect tghese variables to be appended to the possible
# command-line options
override CFLAGS += $(CFLAGS_USR) $(CFLAGS_PLATFORM) $(CFLAGS_DEBUG) $(CPPFLAGS) $(CXXFLAGS)
override LDFLAGS += $(LFLAGS) $(LDFLAGS_PLATFORM)

# Output library names
OUT = $(LIBNAME)

.SECONDEXPANSION:

# Library objects
$(LIBNAME)_OBJS_LIB = $(SRC_DIR)/disptable_core.o $(SRC_DIR)/disptable_err.o

# Objects common for this library
common_OBJS =

# Objects for each version of library
$(LIBNAME)_OBJS = $(common_OBJS) $($(LIBNAME)_OBJS_LIB)
$(LIBNAME)_CODE_HEADERS = \
	$(INCLUDE_DIR)/disptable_classes.h \
	$(INCLUDE_DIR)/disptable_prelude.h \
	$(INCLUDE_DIR)/disptable.h \
	$(INCLUDE_DIR)/disptable_core.h \
	$(INCLUDE_DIR)/disptable_err.h

$(LIBNAME)_HEADERS = $($(LIBNAME)_CODE_HEADERS)

OBJS_all = $(common_OBJS) $($(LIBNAME)_OBJS)

# Libraries suffixes
LIB_STATIC_SUFFIX = .a
LIB_SHARED_SUFFIX = .so

# Generate suitable names for libraries
TARGET_STATIC = $(addsuffix $(LIB_STATIC_SUFFIX), $(OUT))
TARGET_SHARED = $(addsuffix $(LIB_SHARED_SUFFIX), $(OUT))
TARGET_SHARED_VER = $(addsuffix $(LIB_SHARED_SUFFIX).$(LIB_VER), $(OUT))

.PHONY: all clean mrproper install uninstall

# Avoid deletion of intermediate files, such as objects
.SECONDARY: $(OBJS_all)

# Makefile rules
all: $(TARGET_STATIC) $(TARGET_SHARED_VER)

# Compile static library
%.a: $$($$*_OBJS)
	$(AR) rcs $@ $^

# Compile dynamic library
%.so.$(LIB_VER): $$($$*_OBJS) $(OBJ_REVISION)
	$(CC) $(LDFLAGS) -shared -fPIC -Wl,-soname,$@ -o $@ $^

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

install: all
	$(foreach lib,$(TARGET_SHARED_VER),install -m 755 $(lib) $(PREFIX)/lib $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),ln -sf $(lib).$(LIB_VER) $(PREFIX)/lib/$(lib) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),ln -sf $(lib).$(LIB_VER) $(PREFIX)/lib/$(lib).$(LIB_VER_MAJOR) $(CMDSEP))
	$(foreach lib,$(TARGET_STATIC),install -m 755 $(lib) $(PREFIX)/lib $(CMDSEP))
	$(foreach header,$($(LIBNAME)_HEADERS),install -m 755 $(header) $(PREFIX)/include $(CMDSEP))

uninstall:
	$(foreach lib,$(TARGET_SHARED),rm -f $(PREFIX)/lib/$(lib).$(LIB_VER) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),rm -f $(PREFIX)/lib/$(lib) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),rm -f $(PREFIX)/lib/$(lib).$(LIB_VER_MAJOR) $(CMDSEP))
	$(foreach lib,$(TARGET_STATIC),rm -f $(PREFIX)/lib/$(lib) $(CMDSEP))
	$(foreach header,$(notdir $($(LIBNAME)_HEADERS)),rm -f \
		$(PREFIX)/include/$(header) $(CMDSEP))

clean:
	rm -f $(OBJS_all) $(OBJS_all:.o=.d)

mrproper: clean
	rm -f *.a *.so.$(LIB_VER)

