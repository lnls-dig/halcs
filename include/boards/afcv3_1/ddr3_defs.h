#ifndef _DDR3_DEFS_H_
#define _DDR3_DEFS_H_

#include <stddef.h>

#ifdef __SHRINK_AFCV3_DDR_SIZE__
#define __MEM_TOTAL_SIZE                (1ULL << 28) /* 256 MB reserved for position storage */
extern const size_t MEM_TOTAL_SIZE;
#else
#define __MEM_TOTAL_SIZE                (1ULL << 31) /* 2 GB reserved for position storage */
extern const size_t MEM_TOTAL_SIZE;
#endif

#define  __MEM_REGION_SIZE              (__MEM_TOTAL_SIZE / 16)
extern const size_t MEM_REGION_SIZE;

/* FPGA Specific */
extern const size_t DDR3_PAYLOAD_SIZE;        /* In Bytes: Artix7 (AFCv3) */

/* DDR3 Specific */

/* DDR3 for AFCv3 has a 32-bit interface */
extern const size_t DDR3_DATA_WIDTH;           /* In Bits */
extern const size_t DDR3_BYTE_2_BIT;
extern const size_t DDR3_ADDR_WORD_2_BYTE;

#endif
