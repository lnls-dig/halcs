halcsd_DIR = $(SRC_DIR)/apps/halcsd
halcsd_cfg_DIR = $(SRC_DIR)/apps/halcsd

halcsd_OBJS = $(halcsd_DIR)/halcsd.o

halcsd_OUT = halcsd

ifeq ($(WITH_APP_CFG),y)
halcsd_cfg_OBJS = $(halcsd_cfg_DIR)/halcsd_cfg.o
halcsd_cfg_OUT = halcsd_cfg
else
halcsd_cfg_OBJS =
halcsd_cfg_OUT =
endif

halcsd_OUT += $(halcsd_cfg_OUT)

halcsd_all_OUT = halcsd halcsd_cfg
halcsd_all_OBJS = $(halcsd_OBJS) $(halcsd_cfg_OBJS)

halcsd_LIBS = -lbsmp
halcsd_STATIC_LIBS =

halcsd_cfg_LIBS = -lbsmp
halcsd_cfg_STATIC_LIBS =

halcsd_INIT_SYSTEM = $(shell $(TOP)/scripts/get-init-system.sh)

halcsd_INIT_DIR = $(halcsd_DIR)/init/$(halcsd_INIT_SYSTEM)
halcsd_INIT_SPECIFIC_ETC_FILES = $(shell cd $(halcsd_INIT_DIR) && find etc/ -type f ! -name '*.in')
halcsd_INIT_SPECIFIC_SHARE_FILES = $(shell cd $(halcsd_INIT_DIR) && find share/ -type f ! -name '*.in')

halcsd_INIT_GENERIC_DIR = $(halcsd_DIR)/init-generic
halcsd_INIT_GENERIC_ETC_FILES = $(shell cd $(halcsd_INIT_GENERIC_DIR) && find etc/ -type f ! -name '*.in')
halcsd_INIT_GENERIC_SHARE_FILES = $(shell cd $(halcsd_INIT_GENERIC_DIR) && find share/ -type f ! -name '*.in')

# Top Makefile uses $(app)_ETC_SCRIPTS/$(app)_SHARE_SCRIPTS this to install
# config files
halcsd_ETC_SCRIPTS = $(addprefix $(halcsd_INIT_DIR)/:,$(halcsd_INIT_SPECIFIC_ETC_FILES)) \
	$(addprefix $(halcsd_INIT_GENERIC_DIR)/:,$(halcsd_INIT_GENERIC_ETC_FILES))
halcsd_SHARE_SCRIPTS = $(addprefix $(halcsd_INIT_DIR)/:,$(halcsd_INIT_SPECIFIC_SHARE_FILES)) \
	$(addprefix $(halcsd_INIT_GENERIC_DIR)/:,$(halcsd_INIT_GENERIC_SHARE_FILES))
