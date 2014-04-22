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

# Include directories
INCLUDE_DIRS = -I. -Iinclude

# Merge all flags. Optimize for size (-Os)
CFLAGS += $(CFLAGS_PLATFORM) $(CFLAGS_DEBUG)
#-Os

LDFLAGS = $(LDFLAGS_PLATFORM)
#-ffunction-sections -fdata-sections -Wl,--gc-sections

OUT=client

# Makefile rules
all: $(OUT)

client: client.c
	$(CC) $(LFLAGS) $(CFLAGS) $(INCLUDE_DIRS) $^ -o $@ $(LIBS)

#BAD
clean:
	find . -iname "*.o" -exec rm '{}' \;

mrproper: clean
	rm -f $(OUT)
