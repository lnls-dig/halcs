/****************************************************************************
 * core/sm_io/src/sm_io/c/modules/rffe_scpi/sm_io_rffe_scpi_core.h
 *
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

#ifndef _SM_IO_RFFE_SCPI_CORE_H_
#define _SM_IO_RFFE_SCPI_CORE_H_

#define SMIO_CTL_HANDLER(smio_handler) (smio_handler->smch_ctl)

typedef struct {
    smpr_scpi_t *smpr_ctl;
    smch_rffe_scpi_t *smch_ctl;
} smio_rffe_scpi_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_rffe_scpi_t * smio_rffe_scpi_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_rffe_scpi_destroy (smio_rffe_scpi_t **self_p);

#endif
