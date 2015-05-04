LIBBPMCLIENT_DIR = ../../../src/libs/libbpmclient

include $(LIBBPMCLIENT_DIR)/Makefile.inc

# Include directories
INCLUDE_DIRS += -I/usr/include/python2.7

INCLUDE_DIRS_ABS = $(filter -I/%, $(INCLUDE_DIRS))
INCLUDE_DIRS_REL = $(filter-out -I/%, $(INCLUDE_DIRS))

INCLUDE_DIRS_PYTHON = $(addprefix -I$(LIBBPMCLIENT_DIR)/, $(INCLUDE_DIRS_REL:-I%=%))
INCLUDE_DIRS_PYTHON += $(INCLUDE_DIRS_ABS)

# Library objects
$(LIBBPMCLIENT)_OBJS_PYTHON_LIB = bpm_client_wrap.o
$(LIBBPMCLIENT)_SWIG_INPUT = bpm_client.i

# Libraries suffixes
LIB_PYTHON_SO_PREFIX = _
LIB_PYTHON_PY_SUFFIX = .py

# Generate suitable names for static libraries
TARGET_PYTHON_SO = $(addprefix $(LIB_PYTHON_SO_PREFIX), $(TARGET_SHARED))
TARGET_PYTHON_PY = $(addsuffix $(LIB_PYTHON_PY_SUFFIX), $(OUT))

# We need to prefix the OBJS_all variable as this refer to LIBBPMCLIENT_DIR
$(LIBBPMCLIENT)_OBJS_PYTHON = $(addprefix $(LIBBPMCLIENT_DIR)/, $($(LIBBPMCLIENT)_OBJS))
OBJ_REVISION_PYTHON = $(addprefix $(LIBBPMCLIENT_DIR)/, $(OBJS_REVISION))

OBJS_PYTHON_all = $($(LIBBPMCLIENT)_OBJS_PYTHON) $($(LIBBPMCLIENT)_OBJS_PYTHON_LIB) \
				  $(OBJ_REVISION_PYTHON)

.PHONY: all swig_python clean mrproper install uninstall

# Avoid deletion of intermediate files, such as objects
.SECONDARY: $(OBJS_PYTHON_all)

# Makefile rules
all: swig_python $(TARGET_PYTHON_SO)

# Compile dynamic library
_$(LIBBPMCLIENT).so: $($(LIBBPMCLIENT)_OBJS_PYTHON) $(OBJ_REVISION_PYTHON) $($(LIBBPMCLIENT)_OBJS_PYTHON_LIB)
	$(CC) -shared -fPIC -Wl,-soname,$@ -o $@ $^ $(LDFLAGS) $(LIBS)

swig_python:
	swig3.0 -python $($(LIBBPMCLIENT)_SWIG_INPUT)

#$(REVISION_NAME).o: $(REVISION_NAME).c
#	$(CC) $(CFLAGS) -DGIT_REVISION="\"$(GIT_REVISION)\"" \
#        -DGIT_USER_NAME="\"$(GIT_USER_NAME)\"" \
#        -DGIT_USER_EMAIL="\"$(GIT_USER_EMAIL)\"" \
#        -c $<

# Pull in dependency info for *existing* .o files and don't complain if the
# corresponding .d file is not found
-include $(OBJS_PYTHON_all:.o=.d)

# Compile with position-independent objects.
# Autodependencies generatation by Scott McPeak, November 2001,
# from article "Autodependencies with GNU make"
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS_PYTHON) -c $*.c -o $@

# create the dependency files "target: pre-requisites"
	${CC} -MM $(CFLAGS) $(INCLUDE_DIRS_PYTHON) $*.c > $*.d

# Workaround to make objects in different folders have
# the correct target path. e.g., "dir/bar.o: dir/bar.c dir/foo.h"
# instead of "bar.o: dir/bar.c dir/foo.h"
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d

# All prereqs listed will also become command-less,
# prereq-less targets. In this way, the prereq file will be
# treated as changed and the target will be rebuilt
#   sed:    strip the target (everything before colon)
#   sed:    remove any continuation backslashes
#   fmt -1: list words one per line
#   sed:    strip leading spaces
#   sed:    add trailing colons
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
		sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

install:
	$(foreach lib,$(TARGET_PYTHON_SO),install -m 755 $(lib) $(INSTALL_DIR)/lib $(CMDSEP))
	$(foreach lib,$(TARGET_PYTHON_PY),install -m 755 $(lib) $(INSTALL_DIR)/lib $(CMDSEP))

uninstall:
	$(foreach lib,$(TARGET_PYTHON_SO),ls -l $(INSTALL_DIR)/lib $(CMDSEP))
	$(foreach lib,$(TARGET_PYTHON_PY),ls -l $(INSTALL_DIR)/lib $(CMDSEP))

clean:
	rm -f $(OBJS_PYTHON_all) $(OBJS_PYTHON_all:.o=.d) $(REVISION_NAME).o
	$(MAKE) -C $(LIBBPMCLIENT_DIR) clean

mrproper: clean
	rm -f _*.so
	$(MAKE) -C $(LIBBPMCLIENT_DIR) mrproper

