/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Janito Vaqueiro Ferreira Filho <janito.filho@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _BPM_CLIENT_CLASSES_H_
#define _BPM_CLIENT_CLASSES_H_

/* Set up environment for the application */
#include "bpm_client_prelude.h"

/* External dependencies */
#include <malamute.h>
#include <errhand.h>
#include <hutils.h>
#include <disptable.h>
#include <halcs_client.h>
#include <acq_client.h>

/* Opaque bpm_client_t structure */
typedef struct _bpm_single_pass_t bpm_single_pass_t;

/* BPM CLIENT */
#include "bpm_client_core.h"

#endif
