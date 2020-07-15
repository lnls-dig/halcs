ldconf_DIR = $(LDCONF_DIR)

ldconf_SCRIPT_IN = halcs.conf.in
ldconf_SCRIPT = halcs.conf

# configure file
DUMMY := $(shell sed 's/@ldconf_PREFIX@/$(ldconf_PREFIX)/g' $(ldconf_DIR)/$(ldconf_SCRIPT_IN) > $(ldconf_DIR)/$(ldconf_SCRIPT))

# Top Makefile uses $(ldconf)_ETC_SCRIPTS this to install
# config files
ldconf_ETC_SCRIPTS = $(addprefix $(ldconf_DIR)/:,$(ldconf_SCRIPT))
