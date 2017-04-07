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

# General C/CPP flags
CFLAGS_USR = -std=gnu99 -O2
# We expect tghese variables to be appended to the possible
# command-line options
override CPPFLAGS +=
override CXXFLAGS +=

# Malamute 1.0.0 requires this to be defined
# as all of its API is in DRAFT state
CFLAGS_USR += -DMLM_BUILD_DRAFT_API

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
CFLAGS_PLATFORM = -Wall -Wextra -Werror \
		  -Wno-missing-field-initializers \
		  -Wno-missing-braces

ifeq ($(notdir $(CC)),$(filter $(notdir $(CC)),gcc cc))
CFLAGS_PLATFORM += -Wno-cpp
endif

ifeq ($(notdir $(CC)),clang)
CFLAGS_PLATFORM += -Wno-error=\#warnings
endif

LDFLAGS_PLATFORM =

# Libraries
LIBS = -lbpmclient -lacqclient -lhalcsclient -lerrhand -lhutils -lmlm -lczmq -lzmq
# General library flags -L<libdir>
LFLAGS =

# Include directories
INCLUDE_DIRS = -I. -I/usr/local/include

# Merge all flags. We expect tghese variables to be appended to the possible
# command-line options
override CFLAGS += $(CFLAGS_USR) $(CFLAGS_PLATFORM) $(CFLAGS_DEBUG) $(CPPFLAGS) $(CXXFLAGS)
override LDFLAGS += $(LFLAGS) $(LDFLAGS_PLATFORM)

# Every .c file will must be a separate example
examples_DIRS = $(wildcard src/*)
examples_SRC = $(foreach dir,$(examples_DIRS),$(wildcard $(dir)/c/*.c))
OUT = $(basename $(examples_SRC))

all: $(OUT)

%: %.c
	$(CC) $(LDFLAGS) $(CFLAGS) $(INCLUDE_DIRS) $^ -o $@ $(LIBS)

# For compatibility
install:

#BAD
clean:
	find . -iname "*.o" -exec rm '{}' \;

mrproper: clean
	rm -f $(OUT)
