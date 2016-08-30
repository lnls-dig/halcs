/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#ifndef _SDBUTILS_ERR_H_
#define _SDBUTILS_ERR_H_

#ifdef __cplusplus
extern "C" {
#endif

enum _sdbutils_err_e
{
    SDBUTILS_SUCCESS = 0,               /* No error */
    SDBUTILS_ERR_ALLOC,                 /* Could not allocate memory */
    SDBUTILS_ERR_OPEN,                  /* Could not open SDBFS */
    SDBUTILS_ERR_UNK_REC,               /* Unknown record type */
    SDBUTILS_ERR_END
};

typedef enum _sdbutils_err_e sdbutils_err_e;

/* Convert enumeration type to string */
const char * sdbutils_err_str (sdbutils_err_e err);

#ifdef __cplusplus
}
#endif

#endif
