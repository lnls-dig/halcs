#ifndef _DDR3_DEFS_H_
#define _DDR3_DEFS_H_

#define MEM_TOTAL_SIZE                  (1ULL << 31)            /* 2 GB reserved for position storage */
#define MEM_REGION_SIZE                 (MEM_TOTAL_SIZE / 16)

/* FPGA Specific */
#define DDR3_PAYLOAD_SIZE               32                      /* In Bytes: Artix7 (AFCv3) */

/* DDR3 Specific */

/* DDR3 for AFCv3 has a 32-bit interface */
#define DDR3_DATA_WIDTH                 32                      /* In Bits */
#define DDR3_BYTE_2_BIT                 8
#define DDR3_ADDR_WORD_2_BYTE           (DDR3_DATA_WIDTH/DDR3_BYTE_2_BIT)

#endif
