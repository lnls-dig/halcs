/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _DEV_MNGR_DEV_INFO_H_
#define _DEV_MNGR_DEV_INFO_H_

#include "czmq.h"
#include "dev_mngr_err.h"
#include "ll_io_utils.h"            /* LLIO types */
#include "dev_io_utils.h"           /* DEVIO types */

enum _devio_state_e {
    INACTIVE = 0,                       /* Nothing found yet */
    READY_TO_RUN,                       /* Device found but not yet initialized */
    RUNNING,                            /* Device is running */
    STOPPED,                            /* Device is stopped momentarily */
    KILLED                              /* Device is dead. Clean it and restart, if needed */
};

struct _devio_info_t {
    uint32_t id;                        /* Device IO ID */
    llio_type_e type;                   /* Device type */
    devio_type_e devio_type;            /* DEVIO type */
    uint32_t smio_inst_id;              /* Optional SMIO instance ID for DEVIOs
                                           with a single SMIO */
    char *dev_pathname;                 /* /dev pathname */
    enum _devio_state_e state;          /* Device IO state */
};

typedef struct _devio_info_t devio_info_t;
typedef enum _devio_state_e devio_state_e;

devio_info_t * devio_info_new (char *dev_pathname, uint32_t id, llio_type_e type,
        devio_type_e devio_type, uint32_t smio_inst_id, devio_state_e state);
dmngr_err_e devio_info_destroy (devio_info_t **self_p);

#endif
