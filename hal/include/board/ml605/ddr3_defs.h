#ifndef _DDR3_DEFS_H_
#define _DDR3_DEFS_H_

#define MEM_TOTAL_SIZE (1 << 28) // 256 MB
#define MEM_REGION_SIZE (MEM_TOTAL_SIZE / 8)

/* FPGA Specific */
/* Do not touch these unless you know what you are doing! */
#define DDR3_PAYLOAD_SIZE 32 /* In Bytes: Virtex6 (ML605) */

#endif
