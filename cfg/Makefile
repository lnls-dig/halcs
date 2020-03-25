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
CFG_DIR ?= ${PREFIX}/etc/halcs

# Config file must have the following name
CFG_FILENAME = halcs.cfg
# This could be set to a different config folder
CFG ?= crude_defconfig

# Get crude defconfig .cfg file
CFG_FILES = $(CFG)/$(CFG_FILENAME)
# Destination config filenames
DEST_CFG_FILES = $(CFG_DIR)/$(CFG_FILENAME)

# Check if config file exists
ifneq ($(wildcard $(DEST_CFG_FILES)),)
CFG_FILE_EXISTS = 1
else
CFG_FILE_EXISTS = 0
endif

.PHONY: all clean mrproper install uninstall

# Makefile rules
all:

install:
ifeq ($(CFG_FILE_EXISTS),1)
	@echo "Found config file at \"$(DEST_CFG_FILES)\". Skipping installation of config file."
else
	@echo "Config file not found at \"$(DEST_CFG_FILES)\". Installing config file."
	@mkdir -m 755 -p $(CFG_DIR)
	$(foreach cfg,$(CFG_FILES),install -m 644 $(cfg) $(CFG_DIR)/ $(CMDSEP))
endif

uninstall:
	$(foreach cfg,$(CFG_FILES),rm -f $(CFG_DIR)/$(cfg) $(CMDSEP))
	@rm -rf $(CFG_DIR)

clean:

mrproper: clean

