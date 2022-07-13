/*
 *  * Copyright (C) 2020 LNLS (www.lnls.br)
 *   * Author: Melissa Aguiar <melissa.aguiar@cnpem.br>
 *    *
 *     * Released according to the GNU GPL, version 3 or any later version.
 *      */

#ifndef _SM_IO_FOFB_PROCESSING_EXPORTS_H_
#define _SM_IO_FOFB_PROCESSING_EXPORTS_H_

#include "disptable.h"

extern disp_op_t fofb_processing_set_get_ram_write_exp;
extern disp_op_t fofb_processing_set_get_ram_addr_exp;
extern disp_op_t fofb_processing_set_get_ram_data_in_exp;
extern disp_op_t fofb_processing_set_get_ram_data_out_exp;


extern const disp_op_t *fofb_processing_exp_ops [];

#endif