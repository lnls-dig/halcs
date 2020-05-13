/****************************************************************************
 * core/common/include/sm_pr_scpi.h
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

#ifndef _SM_PR_SCPI_H_
#define _SM_PR_SCPI_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************** SMPR proto SCPI methods **********************/


/************************* Our Methods **************************/

/* Creates a new instance of the proto_scpi */
smpr_scpi_t *smpr_scpi_new ();
/* Destroy an instance of the scpi */
smpr_err_e smpr_scpi_destroy (smpr_scpi_t **self_p);

/* Get SCPI operations */
const smpr_proto_ops_t *smpr_scpi_get_ops (smpr_scpi_t *self);

#ifdef __cplusplus
}
#endif

#endif
