
LINUX ?= /lib/modules/$(shell uname -r)/build

# If we compile for the kernel, we need to include real kernel headers.
# The thing is enough a mess that I moved it to a different file
include Makefile.arch


AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
CC              = $(CROSS_COMPILE)gcc
CPP             = $(CC) -E
AR              = $(CROSS_COMPILE)ar
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump

# calculate endianness at compile time
ENDIAN := $(shell ./check-endian $(CC))

override CFLAGS += -Wall -ggdb -O2
override CFLAGS += -I../../core/common/include # for <sdb.h>
override CFLAGS += -Iinclude
override CFLAGS += -ffunction-sections -fdata-sections
override CFLAGS += -Wno-pointer-sign
override CFLAGS += $(ENDIAN) $(LINUXINCLUDE)


LIB = libsdbfs.a
OBJS = src/sdbfs/c/glue.o src/sdbfs/c/access.o

all: $(LIB)

$(OBJS): $(wildcard include/*.h) $(wildcard include/linux/*.h)

$(LIB): $(OBJS)
	$(AR) r $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIB) *~ core

mrproper: clean

# add the other unused targets, so the rule in ../Makefile works
modules install uninstall modules_install:
