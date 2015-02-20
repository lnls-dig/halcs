/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _HAL_UTILS_H_
#define _HAL_UTILS_H_

#include <inttypes.h>

/* Returns the necessary string length including the termianting null character
 * for a number in a arbitrary base */
uint32_t num_to_str_len (uint32_t key, uint32_t base);

/* Returns the necessary string length including the termianting null character
 * for a hexadecimal number */
uint32_t hex_to_str_len (uint32_t key);

/* Returns the necessary string length including the termianting null character
 * for a decimal number */
uint32_t dec_to_str_len (uint32_t key);

/* Allocates a string with the necessary size to fit a number in an arbitrary base */
char *halutils_stringify_key (uint32_t key, uint32_t base);

/* Allocates a string with the necessary size to fit a decimal key */
char *halutils_stringify_dec_key (uint32_t key);

/* Allocates a string with the necessary size to fit an hexadecimal key */
char *halutils_stringify_hex_key (uint32_t key);

/* Concatenates 2 strings together with a separator. returns the string if
 * OK, NULL in case of error */
char *halutils_concat_strings (const char *str1, const char* str2, char sep);

/* Concatenates 2 strings together without a separator. returns the string if
 * OK, NULL in case of error */
char *halutils_concat_strings_no_sep (const char *str1, const char* str2);

/* Concatenates 3 strings together with a separator between the first and second
 * strings. returns the string if OK, NULL in case of error */
char *halutils_concat_strings3 (const char *str1, const char* str2,
        const char* str3, char sep);

#endif
