#ifndef _DDR3_DEFS_H_
#define _DDR3_DEFS_H_

#define MEM_TOTAL_SIZE (1UL << 31) /* 2 GB reserved for position storage */
#define MEM_REGION_SIZE (MEM_TOTAL_SIZE / 16)

/* FPGA Specific */
/* Do not touch these unless you know what you are doing! */
#define DDR3_PAYLOAD_SIZE 32 /* In Bytes: Artix7 (AFCv3) */

#endif
