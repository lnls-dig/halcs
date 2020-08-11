halcs_generic_udev_DIR = $(SRC_DIR)/apps/halcs_generic_udev

halcs_generic_udev_INIT_SYSTEM = $(shell $(TOP)/scripts/get-init-system.sh)

halcs_generic_udev_INIT_DIR = $(halcs_generic_udev_DIR)/init/$(halcs_generic_udev_INIT_SYSTEM)
halcs_generic_udev_INIT_SPECIFIC_ETC_FILES = $(shell cd $(halcs_generic_udev_INIT_DIR) && find etc/ -type f ! -name '*.in')
halcs_generic_udev_INIT_SPECIFIC_SHARE_FILES = $(shell cd $(halcs_generic_udev_INIT_DIR) && find share/ -type f ! -name '*.in')

halcs_generic_udev_INIT_GENERIC_DIR = $(halcs_generic_udev_DIR)/init-generic
halcs_generic_udev_INIT_GENERIC_ETC_FILES = $(shell cd $(halcs_generic_udev_INIT_GENERIC_DIR) && find etc/ -type f ! -name '*.in')
halcs_generic_udev_INIT_GENERIC_SHARE_FILES = $(shell cd $(halcs_generic_udev_INIT_GENERIC_DIR) && find share/ -type f ! -name '*.in')

# Top Makefile uses $(app)_ETC_SCRIPTS/$(app)_SHARE_SCRIPTS this to install
# config files
halcs_generic_udev_ETC_SCRIPTS = $(addprefix $(halcs_generic_udev_INIT_DIR)/:,$(halcs_generic_udev_INIT_SPECIFIC_ETC_FILES)) \
	$(addprefix $(halcs_generic_udev_INIT_GENERIC_DIR)/:,$(halcs_generic_udev_INIT_GENERIC_ETC_FILES))
halcs_generic_udev_SHARE_SCRIPTS = $(addprefix $(halcs_generic_udev_INIT_DIR)/:,$(halcs_generic_udev_INIT_SPECIFIC_SHARE_FILES)) \
	$(addprefix $(halcs_generic_udev_INIT_GENERIC_DIR)/:,$(halcs_generic_udev_INIT_GENERIC_SHARE_FILES))
