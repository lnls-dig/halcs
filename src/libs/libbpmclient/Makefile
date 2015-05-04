include Makefile.inc

.PHONY: all clean mrproper install uninstall

# Avoid deletion of intermediate files, such as objects
.SECONDARY: $(OBJS_all)

# Makefile rules
all: $(TARGET_STATIC) $(TARGET_SHARED_VER)

# Compile static library
%.a: $$($$*_OBJS) $(OBJ_REVISION)
	$(AR) rcs $@ $^

# Compile dynamic library
%.so.$(LIB_VER): $$($$*_OBJS) $(OBJ_REVISION)
	$(CC) -shared -fPIC -Wl,-soname,$@ -o $@ $^ $(LDFLAGS)

$(REVISION_NAME).o: $(REVISION_NAME).c
	$(CC) $(CFLAGS) -DGIT_REVISION="\"$(GIT_REVISION)\"" \
        -DGIT_USER_NAME="\"$(GIT_USER_NAME)\"" \
        -DGIT_USER_EMAIL="\"$(GIT_USER_EMAIL)\"" \
        -c $<

# Pull in dependency info for *existing* .o files and don't complain if the
# corresponding .d file is not found
-include $(OBJS_all:.o=.d)

# Compile with position-independent objects.
# Autodependencies generatation by Scott McPeak, November 2001,
# from article "Autodependencies with GNU make"
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $*.c -o $@

# create the dependency files "target: pre-requisites"
	${CC} -MM $(CFLAGS) $(INCLUDE_DIRS) $*.c > $*.d

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

#tests:
#	$(MAKE) -C $@ all
#

install:
	$(foreach lib,$(TARGET_SHARED_VER),install -m 755 $(lib) $(INSTALL_DIR)/lib $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),ln -sf $(lib).$(LIB_VER) $(INSTALL_DIR)/lib/$(lib) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),ln -sf $(lib).$(LIB_VER) $(INSTALL_DIR)/lib/$(lib).$(LIB_VER_MAJOR) $(CMDSEP))
	$(foreach lib,$(TARGET_STATIC),install -m 755 $(lib) $(INSTALL_DIR)/lib $(CMDSEP))
	$(foreach header,$($(LIBBPMCLIENT)_INST_HEADERS),install -m 755 $(header) $(INSTALL_DIR)/include $(CMDSEP))

uninstall:
	$(foreach lib,$(TARGET_SHARED),rm -f $(INSTALL_DIR)/lib/$(lib).$(LIB_VER) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),rm -f $(INSTALL_DIR)/lib/$(lib) $(CMDSEP))
	$(foreach lib,$(TARGET_SHARED),rm -f $(INSTALL_DIR)/lib/$(lib).$(LIB_VER_MAJOR) $(CMDSEP))
	$(foreach lib,$(TARGET_STATIC),rm -f $(INSTALL_DIR)/lib/$(lib) $(CMDSEP))
	$(foreach header,$(notdir $($(LIBBPMCLIENT)_INST_HEADERS)),rm -f \
		$(INSTALL_DIR)/include/$(header) $(CMDSEP))

clean:
	rm -f $(OBJS_all) $(OBJS_all:.o=.d) $(REVISION_NAME).o

mrproper: clean
	rm -f *.a *.so.$(LIB_VER)

