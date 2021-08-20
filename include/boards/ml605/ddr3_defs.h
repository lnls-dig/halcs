#ifndef _DDR3_DEFS_H_
#define _DDR3_DEFS_H_

#include <stddef.h>

#define __MEM_TOTAL_SIZE                (1ULL << 28) /* 256 MB reserved for position storage */
#define  __MEM_REGION_SIZE              (__MEM_TOTAL_SIZE / 8)

#endif
