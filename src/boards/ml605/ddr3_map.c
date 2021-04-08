/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "acq_chan_gen_defs.h"
#include "ddr3_map_structs.h"
#include "boards/ml605/ddr3_map.h"

#include <stddef.h>

/******************************************************/
/*                   Memory definitions               */
/******************************************************/

const size_t MEM_TOTAL_SIZE             = __MEM_TOTAL_SIZE;

const size_t MEM_REGION_SIZE            = __MEM_REGION_SIZE;
/* FPGA Specific */
const size_t DDR3_PAYLOAD_SIZE          = 32;                     /* In Bytes: Virtex6 (ML605) */

/* DDR3 Specific */

/* DDR3 for ML605 has a 64-bit interface */
#define __DDR3_DATA_WIDTH               64
const size_t DDR3_DATA_WIDTH            = __DDR3_DATA_WIDTH;      /* In Bits */
#define __DDR3_BYTE_2_BIT               8
const size_t DDR3_BYTE_2_BIT            = __DDR3_BYTE_2_BIT;
const size_t DDR3_ADDR_WORD_2_BYTE      = (__DDR3_DATA_WIDTH/__DDR3_BYTE_2_BIT);
