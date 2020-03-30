
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
MAKE            = make

CMDSEP = ;

# install prefix
PREFIX ?= /usr

# calculate endianness at compile time
ENDIAN := $(shell ./check-endian $(CC))

override CFLAGS += -Wall -ggdb -O2
override CFLAGS += -Iinclude -I../../libs/llio/include
override CFLAGS += -ffunction-sections -fdata-sections
override CFLAGS += -Wno-pointer-sign
override CFLAGS += $(ENDIAN) $(LINUXINCLUDE)

.PHONY: tools tools_clean tools_install tools_uninstall

LIB = libsdbfs.a
OBJS = src/sdbfs/c/glue.o src/sdbfs/c/access.o

all: $(LIB) tools

$(OBJS): $(wildcard include/*.h) $(wildcard include/linux/*.h)

$(LIB): $(OBJS)
	$(AR) r $@ $(OBJS)

tools:
	$(MAKE) -C tools

clean: tools_clean
	rm -f $(OBJS) $(LIB) *~ core

tools_clean:
	$(MAKE) -C tools clean

mrproper: clean

install: tools_install
	$(foreach lib,$(LIB),install -m 755 $(lib) $(PREFIX)/lib/ $(CMDSEP))

uninstall: tools_uninstall
	$(foreach lib,$(LIB),rm -f $(PREFIX)/lib/$(lib) $(CMDSEP))

tools_install:
	$(MAKE) -C tools install

tools_uninstall:
	$(MAKE) -C tools uninstall

# add the other unused targets, so the rule in ../Makefile works
modules modules_install:
