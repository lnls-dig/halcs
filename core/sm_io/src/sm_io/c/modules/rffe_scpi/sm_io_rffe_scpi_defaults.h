/****************************************************************************
 * core/sm_io/src/sm_io/c/modules/rffe_scpi/sm_io_rffe_scpi_defaults.h
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

#ifndef _RFFE_SCPI_DEFAULTS_H_
#define _RFFE_SCPI_DEFAULTS_H_

#define RFFE_DFLT_ATT                               31.5            /* 31.1 dB attenuation */

smio_err_e rffe_scpi_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif
