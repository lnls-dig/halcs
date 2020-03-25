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
# This SCRIPTS_ETC_PREFIX could be different than PREFIX, as SCRIPTS_ETC_PREFIX
# is defined as the location of init scripts and must be placed in a specific
# place. The main use of thuis variable is for testing the build system
SCRIPTS_ETC_PREFIX ?=
SCRIPTS_SHARE_PREFIX ?= /usr/local

# OS information
SERVICE_NAME = halcs
INIT_SYSTEM := init-generic
SCRIPTS_ETC_FULL_NAME := $(shell cd $(INIT_SYSTEM) && find etc/ -type f)
SCRIPTS_SHARE_FULL_NAME := $(shell cd $(INIT_SYSTEM) && find share/ -type f)

# Strip off a leading ./
SCRIPTS_ETC=$(SCRIPTS_ETC_FULL_NAME:./%=%)
SCRIPTS_SHARE=$(SCRIPTS_SHARE_FULL_NAME:./%=%)

.PHONY: all clean mrproper install uninstall

all:

# Makefile rules
install:
	$(foreach script,$(SCRIPTS_ETC),mkdir -p $(dir ${SCRIPTS_ETC_PREFIX}/$(script)) $(CMDSEP))
	$(foreach script,$(SCRIPTS_ETC),cp --preserve=mode $(INIT_SYSTEM)/$(script) ${SCRIPTS_ETC_PREFIX}/$(script) $(CMDSEP))
	$(foreach script,$(SCRIPTS_SHARE),mkdir -p $(dir ${SCRIPTS_SHARE_PREFIX}/$(script)) $(CMDSEP))
	$(foreach script,$(SCRIPTS_SHARE),cp --preserve=mode $(INIT_SYSTEM)/$(script) ${SCRIPTS_SHARE_PREFIX}/$(script) $(CMDSEP))

uninstall:
	$(foreach script,$(SCRIPTS_SHARE),rm -f ${SCRIPTS_SHARE_PREFIX}/$(script) $(CMDSEP))
	$(foreach script,$(SCRIPTS_ETC),rm -f ${SCRIPTS_ETC_PREFIX}/$(script) $(CMDSEP))

clean:

mrproper: clean
