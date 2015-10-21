
AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
CC              = $(CROSS_COMPILE)gcc
CPP             = $(CC) -E
AR              = $(CROSS_COMPILE)ar
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump

ERRHAND_DBG ?= y
ERRHAND_MIN_LEVEL ?= DBG_LVL_ERR
ERRHAND_SUBSYS_ON ?= "(DBG_DEV_MNGR | DBG_DEV_IO | DBG_SM_IO | DBG_LIB_CLIENT | DBG_SM_PR | DBG_SM_CH | DBG_LL_IO | DBG_HAL_UTILS)"

CFLAGS = -Wall -ggdb
CFLAGS += -I.. -I../../../include
CFLAGS_LNLS = -DERRHAND_DBG=$(ERRHAND_DBG) -DERRHAND_MIN_LEVEL=$(ERRHAND_MIN_LEVEL) \
	-DERRHAND_SUBSYS_ON=$(ERRHAND_SUBSYS_ON)
LDFLAGS = -L.. -lsdbfs
LDFLAGS_LNLS = -lllio -lerrhand -lconvc -lczmq -lpcidriver

PROG = gensdbfs sdb-read sdb-read-lnls sdb-extract

all: $(PROG)

%: %.c
	$(CC) $(CFLAGS) -o $@ $*.c $(LDFLAGS)

sdb-read-lnls: sdb-read-lnls.c
	$(CC) $(CFLAGS) $(CFLAGS_LNLS) -o $@ $< $(LDFLAGS) $(LDFLAGS_LNLS)

$(PROG): ../libsdbfs.a

clean:
	rm -f $(PROG) *.o *~ core

# add the other unused targets, so the rule in ../Makefile works
modules install modules_install:
