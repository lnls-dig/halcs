halcsd_DIR = $(SRC_DIR)/apps/halcsd/src/halcsd/c
halcsd_cfg_DIR = $(SRC_DIR)/apps/halcsd/src/halcsd_cfg/c

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

halcsd_SCRIPT_FILES = $(shell cd $(halcsd_DIR)/../sysfiles && find . -type f)
halcsd_SCRIPTS = $(patsubst ./%,$(halcsd_DIR)/../sysfiles/:%,$(halcsd_SCRIPT_FILES))

halcsd_INIT_SYSTEM = $(shell $(SRC_DIR)/scripts/get-init-system.sh)

LEDBG = before $(halcsd_INIT_SYSTEM)

ifeq ($(halcsd_INIT_SYSTEM),systemd)
LEDBG += inside
halcsd_SYSTEMD_DIR = $(halcsd_DIR)/../../systemd
halcsd_SYSTEMD_FILES = $(shell cd $(halcsd_SYSTEMD_DIR)/sysfiles && find . -type f)
halcsd_SCRIPTS += $(patsubst ./%,$(halcsd_SYSTEMD_DIR)/sysfiles/:%,$(halcsd_SYSTEMD_FILES))
endif
LEDBG += after
