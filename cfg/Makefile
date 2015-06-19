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

PREFIX ?= /usr/local
CFG_DIR ?= ${PREFIX}/etc/bpm_sw

# Config file must have the following name
CFG_FILENAME = bpm_sw.cfg
# This could be set to a different config folder
CFG ?= crude_defconfig

# Get crude defconfig .cfg file
CFG_FILES = $(CFG)/$(CFG_FILENAME)

.PHONY: all clean mrproper install uninstall

# Makefile rules
all:

install:
	@mkdir -m 755 -p $(CFG_DIR)
	$(foreach cfg,$(CFG_FILES),install -m 755 $(cfg) $(CFG_DIR)/ $(CMDSEP))

uninstall:
	$(foreach cfg,$(CFG_FILES),rm -f $(CFG_DIR)/$(cfg) $(CMDSEP))
	@rm -rf $(CFG_DIR)

clean:

mrproper: clean

