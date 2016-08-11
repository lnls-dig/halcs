/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _DEV_MNGR_DEV_INFO_H_
#define _DEV_MNGR_DEV_INFO_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    INACTIVE = 0,                       /* Nothing found yet */
    READY_TO_RUN,                       /* Device found but not yet initialized */
    RUNNING,                            /* Device is running */
    STOPPED,                            /* Device is stopped momentarily */
    KILLED                              /* Device is dead. Clean it and restart, if needed */
} devio_state_e;

/***************** Our methods *****************/

/* Creates a new instance of Device Information */
devio_info_t * devio_info_new (char *dev_pathname, uint32_t id, llio_type_e type,
        devio_type_e devio_type, uint32_t smio_inst_id, devio_state_e state);
/* Destroy an instance of the Device Information */
dmngr_err_e devio_info_destroy (devio_info_t **self_p);

/* Set Device Info ID */
dmngr_err_e devio_info_set_id (devio_info_t *self, uint32_t id);
/* Get Device Info ID */
uint32_t devio_info_get_id (devio_info_t *self);
/* Set Device Info LLIO type */
dmngr_err_e devio_info_set_llio_type (devio_info_t *self, llio_type_e llio_type);
/* Get Device Info LLIO type */
llio_type_e devio_info_get_llio_type (devio_info_t *self);
/* Set Device Info DEVIO type */
dmngr_err_e devio_info_set_devio_type (devio_info_t *self, devio_type_e devio_type);
/* Get Device Info DEVIO type */
devio_type_e devio_info_get_devio_type (devio_info_t *self);
/* Set Device Info SMIO instance ID */
dmngr_err_e devio_info_set_smio_inst_id (devio_info_t *self, uint32_t smio_inst_id);
/* Get Device Info SMIO instance ID */
uint32_t devio_info_get_smio_inst_id (devio_info_t *self);
/* Set Device Info Device Pathname */
dmngr_err_e devio_info_set_dev_pathname (devio_info_t *self, const char *dev_pathname);
/* Get Device Info Device Pathname */
const char *devio_info_get_dev_pathname (devio_info_t *self);
/* Clone Device Info Device Pathname */
char *devio_info_clone_dev_pathname (devio_info_t *self);
/* Set Device Info Device state */
dmngr_err_e devio_info_set_state (devio_info_t *self, devio_state_e state);
/* Get Device Info Device state */
devio_state_e devio_info_get_state (devio_info_t *self);

#ifdef __cplusplus
}
#endif

#endif
