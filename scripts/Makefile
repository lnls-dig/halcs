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
# This SCRIPTS_PREFIX could be different than PREFIX, as SCRIPTS_PREFIX
# is defined as the location of init scripts and must be placed in a specific
# place. The main use of thuis variable is for testing the build system
SCRIPTS_PREFIX ?=

PWD = 		$(shell pwd)

# OS information
SERVICE_NAME = bpm-sw
INIT_SYSTEM := $(shell ./get-init-system.sh)
SCRIPTS_FULL_NAME := $(shell cd $(INIT_SYSTEM) && find . -type f)

# Strip off a leading ./
SCRIPTS=$(SCRIPTS_FULL_NAME:./%=%)

.PHONY: all clean mrproper install uninstall

all:

# Makefile rules
install:
	$(foreach script,$(SCRIPTS),mkdir -p $(dir ${SCRIPTS_PREFIX}/$(script)) $(CMDSEP))
	$(foreach script,$(SCRIPTS),cp --preserve=mode $(INIT_SYSTEM)/$(script) ${SCRIPTS_PREFIX}/$(script) $(CMDSEP))
	./enable-service.sh -s $(SERVICE_NAME)

uninstall:
	./disable-service.sh -s $(SERVICE_NAME)
	$(foreach script,$(SCRIPTS),rm -f ${SCRITS_PREFIX}/$(script) $(CMDSEP))

clean:

mrproper: clean

