/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _LL_IO_ENDPOINT_H_
#define _LL_IO_ENDPOINT_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************** Our methods *****************/

/* Creates a new instance of the Endpoint*/
llio_endpoint_t * llio_endpoint_new (const char *endpoint);
/* Destroy an instance of the Endpoint */
llio_err_e llio_endpoint_destroy (llio_endpoint_t **self_p);

/* Set endpoint open status */
llio_err_e llio_endpoint_set_open (llio_endpoint_t *self, bool open);
/* Get endpoint open status */
bool llio_endpoint_get_open (llio_endpoint_t *self);
/* Set endpoint to llio instance in case it was not done in llio_endpoint_new() */
llio_err_e llio_endpoint_set_name (llio_endpoint_t *self, const char *endpoint);
/* Get endpoint from name from instance of llio_endpoint */
const char *llio_endpoint_get_name (llio_endpoint_t *self);
/* Clone endpoint from name from instance of llio_endpoint */
char *llio_endpoint_clone_name (llio_endpoint_t *self);

#ifdef __cplusplus
}
#endif

#endif
