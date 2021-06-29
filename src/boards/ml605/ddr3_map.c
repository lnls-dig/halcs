/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "acq_chan_gen_defs.h"
#include "ddr3_map_structs.h"
#include "boards/ml605/ddr3_map.h"

// for HUTILS_EXPORT_SYMBOL
#include "hutils.h"

#include <stddef.h>

/******************************************************/
/*                   Memory definitions               */
/******************************************************/

const size_t ml605_mem_total_size             = __MEM_TOTAL_SIZE;
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, ml605_mem_total_size);

const size_t ml605_mem_region_size            = __MEM_REGION_SIZE;
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, ml605_mem_region_size);
/* FPGA Specific */
const size_t ml605_ddr3_payload_size          = 32;                     /* In Bytes: Virtex6 (ML605) */
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, ml605_ddr3_payload_size);

/* DDR3 Specific */

/* DDR3 for ML605 has a 64-bit interface */
#define __DDR3_DATA_WIDTH               64
const size_t ml605_ddr3_data_width            = __DDR3_DATA_WIDTH;      /* In Bits */
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, ml605_ddr3_data_width);
#define __DDR3_BYTE_2_BIT               8
const size_t ml605_ddr3_byte_2_bit            = __DDR3_BYTE_2_BIT;
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, ml605_ddr3_byte_2_bit);
const size_t ml605_ddr3_addr_word_2_byte      = (__DDR3_DATA_WIDTH/__DDR3_BYTE_2_BIT);
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, ml605_ddr3_addr_word_2_byte);
