
AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
CC              = $(CROSS_COMPILE)gcc
CPP             = $(CC) -E
AR              = $(CROSS_COMPILE)ar
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump

CMDSEP = ;

# install prefix
PREFIX ?= /usr

ERRHAND_DBG ?= y
ERRHAND_MIN_LEVEL ?= DBG_LVL_ERR
ERRHAND_SUBSYS_ON ?= "(DBG_DEV_MNGR | DBG_DEV_IO | DBG_SM_IO | DBG_LIB_CLIENT | DBG_SM_PR | DBG_SM_CH | DBG_LL_IO | DBG_HAL_UTILS)"

override CFLAGS += -Wall -ggdb
override CFLAGS += -I.. \
	               -I../include \
	               -I$(LIBLLIO_DIR)/include \
	               -I$(LIBERRHAND_DIR)/include \
	               -I$(LIBCONVC_DIR)/include
CFLAGS_LNLS = -DERRHAND_DBG=$(ERRHAND_DBG) -DERRHAND_MIN_LEVEL=$(ERRHAND_MIN_LEVEL) \
	-DERRHAND_SUBSYS_ON=$(ERRHAND_SUBSYS_ON)
override LDFLAGS += -L.. \
	               -L$(LIBLLIO_DIR) \
	               -L$(LIBERRHAND_DIR) \
	               -L$(LIBCONVC_DIR) \
	               -L$(PCIE_DRIVER_DIR)/lib/pcie \
	               -lsdbfs
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

install:
	$(foreach prog,$(PROG),install -m 755 $(prog) $(PREFIX)/bin/ $(CMDSEP))

uninstall:
	$(foreach prog,$(PROG),rm -f $(PREFIX)/bin/$(prog) $(CMDSEP))

# add the other unused targets, so the rule in ../Makefile works
modules modules_install:
