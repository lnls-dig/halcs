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
PREFIX ?=

PWD = 		$(shell pwd)

# OS information
SERVICE_NAME = bpm-sw
INIT_SYSTEM = $(shell ./get-init-system.sh)
SCRIPTS_FULL_NAME = $(shell cd $(INIT_SYSTEM) && find . -type f)

# Strip off a leading ./
SCRIPTS=$(SCRIPTS_FULL_NAME:./%=%)

.PHONY: all clean mrproper install uninstall

all:

# Makefile rules
install:
	$(foreach script,$(SCRIPTS),cp --preserve=mode $(INIT_SYSTEM)/$(script) ${PREFIX}/$(script) $(CMDSEP))
	./enable-service.sh $(SERVICE_NAME)

uninstall:
	./disable-service.sh $(SERVICE_NAME)
	$(foreach script,$(SCRIPTS),rm -f ${PREFIX}/$(script) $(CMDSEP))

clean:

mrproper: clean

