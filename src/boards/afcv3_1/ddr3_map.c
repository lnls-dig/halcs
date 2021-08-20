/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "acq_chan_gen_defs.h"
#include "ddr3_map_structs.h"
#include "boards/afcv3_1/ddr3_map.h"

// for HUTILS_EXPORT_SYMBOL
#include "hutils.h"

#include <stddef.h>

/******************************************************/
/*                   Memory definitions               */
/******************************************************/

const size_t afcv3_1_mem_total_size             = __MEM_TOTAL_SIZE;
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, afcv3_1_mem_total_size);

const size_t afcv3_1_mem_region_size            = __MEM_REGION_SIZE;
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, afcv3_1_mem_region_size);
/* FPGA Specific */
const size_t afcv3_1_ddr3_payload_size          = 32;                     /* In Bytes: Artix7 (AFCv3) */
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, afcv3_1_ddr3_payload_size);

/* DDR3 Specific */

/* DDR3 for AFCv3 has a 32-bit interface */
#define __DDR3_DATA_WIDTH               32
const size_t afcv3_1_ddr3_data_width            = __DDR3_DATA_WIDTH;      /* In Bits */
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, afcv3_1_ddr3_data_width);
#define __DDR3_BYTE_2_BIT               8
const size_t afcv3_1_ddr3_byte_2_bit            = __DDR3_BYTE_2_BIT;
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, afcv3_1_ddr3_byte_2_bit);
const size_t afcv3_1_ddr3_addr_word_2_byte      = (__DDR3_DATA_WIDTH/__DDR3_BYTE_2_BIT);
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, afcv3_1_ddr3_addr_word_2_byte);
