/****************************************************************************
 * core/common/include/sm_ch_rffe_scpi.h
 *
 *   Copyright (C) 2014 LNLS (www.lnls.br). All rights reserved.
 *   Copyright (C) 2019 Augusto Fraga Giachero. All rights reserved.
 *   Author: Augusto Fraga Giachero <afg@augustofg.net>
 *
 * This file is part of HALCS.
 *
 * HALCS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HALCS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HALCS. If not, see <https://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#ifndef _SM_CH_RFFE_SCPI_H_
#define _SM_CH_RFFE_SCPI_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************** Our methods *****************/

/* Creates a new instance of the SMCH RFFE */
smch_rffe_scpi_t * smch_rffe_scpi_new (smio_t *parent, const smpr_proto_ops_t *reg_ops, int verbose);
/* Destroy an instance of the SMCH RFFE */
smch_err_e smch_rffe_scpi_destroy (smch_rffe_scpi_t **self_p);

/* Write RFFE variable */
smch_err_e smch_rffe_scpi_write_var (smch_rffe_scpi_t *self, uint32_t id, uint8_t *data,
        size_t size);
/* Read RFFE variable */
smch_err_e smch_rffe_scpi_read_var (smch_rffe_scpi_t *self, uint32_t id, uint8_t *data,
        size_t size);

#ifdef __cplusplus
}
#endif

#endif
