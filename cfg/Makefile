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

CFG_DIR ?= /etc/bpm_sw

# Get all .cfg files
CFG_FILES ?= $(wildcard *.cfg)

.PHONY: all clean mrproper install uninstall

# Makefile rules
all:

install:
	@mkdir -p $(CFG_DIR)
	$(foreach cfg,$(CFG_FILES),install -m 755 $(cfg) $(CFG_DIR)/ $(CMDSEP))

uninstall:
	$(foreach cfg,$(CFG_FILES),rm -f $(CFG_DIR)/$(cfg) $(CMDSEP))
	@rm -rf $(CFG_DIR)

clean:

mrproper: clean

