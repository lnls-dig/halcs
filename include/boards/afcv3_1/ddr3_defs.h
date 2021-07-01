#ifndef _DDR3_DEFS_H_
#define _DDR3_DEFS_H_

#include <stddef.h>

#ifdef __SHRINK_AFCV3_DDR_SIZE__
#define __MEM_TOTAL_SIZE                (1ULL << 28) /* 256 MB reserved for position storage */
#else
#define __MEM_TOTAL_SIZE                (1ULL << 31) /* 2 GB reserved for position storage */
#endif

#define  __MEM_REGION_SIZE              (__MEM_TOTAL_SIZE / 16)

#endif
