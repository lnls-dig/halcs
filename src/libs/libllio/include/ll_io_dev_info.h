/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _LL_IO_DEV_INFO_H_
#define _LL_IO_DEV_INFO_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************** Our methods *****************/

/* Creates a new instance of Device Information */
llio_dev_info_t * llio_dev_info_new ();

/* Destroy an instance of the Device Information */
llio_err_e llio_dev_info_destroy (llio_dev_info_t **self_p);

#ifdef __cplusplus
}
#endif

#endif
