/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _HAL_UTILS_H_
#define _HAL_UTILS_H_

#include <inttypes.h>

uint32_t num_to_str_len (uint32_t key);
char *halutils_stringify_key (uint32_t key);

#endif
