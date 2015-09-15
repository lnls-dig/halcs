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

LIBCONVC = libconvc

# General C/CPP flags
CFLAGS_USR = -std=gnu99 -O2
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

# Include directories
INCLUDE_DIRS = -I. -I../liberrhand

# Merge all flags. We expect tghese variables to be appended to the possible
# command-line options
override CFLAGS += $(CFLAGS_USR) $(CFLAGS_PLATFORM) $(CFLAGS_DEBUG) $(CPPFLAGS) $(CXXFLAGS)
override LDFLAGS += $(LFLAGS) $(LDFLAGS_PLATFORM)

# Output library names
OUT = $(LIBCONVC)

.SECONDEXPANSION:

# Library objects
$(LIBCONVC)_OBJS_LIB = convc.o convc_err.o

# Objects common for this library
common_OBJS =

# Objects for each version of library
$(LIBCONVC)_OBJS = $(common_OBJS) $($(LIBCONVC)_OBJS_LIB)
$(LIBCONVC)_CODE_HEADERS =

$(LIBCONVC)_HEADERS = $($(LIBCONVC)_OBJS_LIB:.o=.h) $($(LIBCONVC)_CODE_HEADERS)

OBJS_all = $(common_OBJS) $($(LIBCONVC)_OBJS)

# Libraries suffixes
LIB_STATIC_SUFFIX = .a

# Generate suitable names for static libraries
TARGET_STATIC = $(addsuffix $(LIB_STATIC_SUFFIX), $(OUT))

.PHONY: all clean mrproper install uninstall

# Avoid deletion of intermediate files, such as objects
.SECONDARY: $(OBJS_all)

# Makefile rules
all: $(TARGET_STATIC)

# Compile static library
%.a: $$($$*_OBJS)
	$(AR) rcs $@ $^

# Pull in dependency info for *existing* .o files and don't complain if the
# corresponding .d file is not found
-include $(OBJS_all:.o=.d)

# Compile with position-independent objects.
# Autodependencies generatation by Scott McPeak, November 2001,
# from article "Autodependencies with GNU make"
%.o: %.c
	$(CC) $(LDFLAGS) $(CFLAGS) $(INCLUDE_DIRS) -c $*.c -o $@

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

install:

uninstall:

clean:
	rm -f $(OBJS_all) $(OBJS_all:.o=.d)

mrproper: clean
	rm -f *.a

