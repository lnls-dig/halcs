#ifndef _MEM_LAYOUT_H_
#define _MEM_LAYOUT_H_

#include "device_mapping.h"

/*********************** Static ML605 FPGA layout ***********************/

extern const size_t SDB_ADDRESS;

/********************* General device mapping structures ******************/

#define __NUM_MAX_SLOTS                 1 
extern const size_t NUM_MAX_SLOTS;
#define __NUM_MAX_HALCS_PER_SLOT        2
extern const size_t NUM_MAX_HALCS_PER_SLOT;
#define __NUM_MAX_HALCSS                (__NUM_MAX_SLOTS * __NUM_MAX_HALCS_PER_SLOT)
extern const size_t NUM_MAX_HALCSS;

/* Device mapping definitions */
extern const board_device_map_t board_device_map[__NUM_MAX_HALCSS+1];

#endif

