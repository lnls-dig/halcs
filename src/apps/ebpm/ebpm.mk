ebpm_DIR = $(SRC_DIR)/apps/ebpm

ebpm_OBJS = $(ebpm_DIR)/ebpm.o

ebpm_OUT = ebpm

ifeq ($(WITH_APP_CFG),y)
ebpm_cfg_OBJS = $(ebpm_DIR)/ebpm_cfg.o
ebpm_cfg_OUT = ebpm_cfg
else
ebpm_cfg_OBJS =
ebpm_cfg_OUT =
endif

ebpm_OUT += $(ebpm_cfg_OUT)

ebpm_all_OUT = ebpm ebpm_cfg
ebpm_all_OBJS = $(ebpm_OBJS) $(ebpm_cfg_OBJS)

ebpm_LIBS = -lbsmp
ebpm_STATIC_LIBS =

ebpm_cfg_LIBS = -lbsmp
ebpm_cfg_STATIC_LIBS =
