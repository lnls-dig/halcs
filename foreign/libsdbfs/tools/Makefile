
AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
CC              = $(CROSS_COMPILE)gcc
CPP             = $(CC) -E
AR              = $(CROSS_COMPILE)ar
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump

CFLAGS = -Wall -ggdb
CFLAGS += -I.. -I../../../include
LDFLAGS = -L.. -lsdbfs

PROG = gensdbfs sdb-read sdb-extract

all: $(PROG)

%: %.c
	$(CC) $(CFLAGS) -o $@ $*.c $(LDFLAGS)

$(PROG): ../libsdbfs.a

clean:
	rm -f $(PROG) *.o *~ core

# add the other unused targets, so the rule in ../Makefile works
modules install modules_install:
