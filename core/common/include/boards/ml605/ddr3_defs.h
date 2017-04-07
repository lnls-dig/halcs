#ifndef _DDR3_DEFS_H_
#define _DDR3_DEFS_H_

#define MEM_TOTAL_SIZE                  (1UL << 28)             /* 256 MB */
#define MEM_REGION_SIZE                 (MEM_TOTAL_SIZE / 8)

/* FPGA Specific */
#define DDR3_PAYLOAD_SIZE               32                      /* In Bytes: Virtex6 (ML605) */

/* DDR3 Specific */

/* DDR3 for AFCv3 has a 32-bit interface */
#define DDR3_DATA_WIDTH                 64                      /* In Bits */
#define DDR3_BYTE_2_BIT                 8
#define DDR3_ADDR_WORD_2_BYTE           (DDR3_DATA_WIDTH/DDR3_BYTE_2_BIT)

#endif
