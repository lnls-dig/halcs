/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _HUTILS_UTILS_H_
#define _HUTILS_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************* Configuration file property names ************************/

#define HUTILS_FE_CFG_ENDP_MAX_PATH         256
#define HUTILS_FE_CFG_ENDP_PATH_PATTERN     "/dev_io/board%u/halcs%u/afe/bind"

/* We don't expect our hash key to be bigger than this */
#define HUTILS_CFG_HASH_KEY_MAX_LEN         64
/* Our config hash key is composed of this pattern: board%u/halcs%u/afe
 * or board%u/halcs%u/dbe */
#define HUTILS_CFG_BOARD_TYPE               "%s"
#define HUTILS_CFG_BOARD_PATTERN            "board%u"
#define HUTILS_CFG_HALCS_TYPE                 "%s"
#define HUTILS_CFG_HALCS_PATTERN              "halcs%u"
#define HUTILS_CFG_DEVIO_MODEL_TYPE         "%s"

#define HUTILS_CFG_HASH_KEY_PATTERN         HUTILS_CFG_BOARD_TYPE \
                                            "/" HUTILS_CFG_HALCS_TYPE
#define HUTILS_CFG_HASH_KEY_PATTERN_COMPL   HUTILS_CFG_BOARD_PATTERN \
                                            "/" HUTILS_CFG_HALCS_PATTERN

typedef struct {
    char *bind;                 /* AFE Endpoint address to bind to */
    char *proto;                /* RFFE protocol (scpi / bsmp) */
    char *board_type;           /* DBE board type (e.g., afcv3.1) */
} hutils_hints_t;

/************************ Our methods *****************************/

/* Returns the necessary string length including the termianting null character
 * for a number in a arbitrary base */
uint32_t hutils_num_to_str_len (uint32_t key, uint32_t base);

/* Returns the necessary string length including the termianting null character
 * for a hexadecimal number */
uint32_t hutils_hex_to_str_len (uint32_t key);

/* Returns the necessary string length including the termianting null character
 * for a decimal number */
uint32_t hutils_dec_to_str_len (uint32_t key);

/* Allocates a string with the necessary size to fit a number in an arbitrary base */
char *hutils_stringify_key (uint32_t key, uint32_t base);

/* Allocates a string with the necessary size to fit a decimal key */
char *hutils_stringify_dec_key (uint32_t key);

/* Allocates a string with the necessary size to fit an hexadecimal key */
char *hutils_stringify_hex_key (uint32_t key);

/* Converts a key string into the specified numeric base. Must fit into
 * a uint32_t */
uint32_t hutils_numerify_key (const char *key, uint32_t base);

/* Converts a key string into the decimal base. Result must fit into
 * a uint32_t */
uint32_t hutils_numerify_dec_key (const char *key);

/* Converts a key string into the hexadecimal base. Result must fit into
 * a uint32_t */
uint32_t hutils_numerify_hex_key (const char *key);

/* Concatenates 2 strings together with a separator. returns the string if
 * OK, NULL in case of error */
char *hutils_concat_strings (const char *str1, const char* str2, char sep);

/* Concatenates 2 strings together without a separator. returns the string if
 * OK, NULL in case of error */
char *hutils_concat_strings_no_sep (const char *str1, const char* str2);

/* Concatenates 3 strings together with a separator between the first and second
 * strings. returns the string if OK, NULL in case of error */
char *hutils_concat_strings3 (const char *str1, const char* str2,
        const char* str3, char sep);

/* Concatenates 3 strings together. Returns the string if OK, NULL in case of error */
char *hutils_concat_strings3_no_sep (const char *str1, const char* str2,
        const char* str3);

/* Calculates necessary padding so that a given value is a multiple of a given
 * alignment */
uint32_t hutils_calculate_padding(uint32_t value, uint32_t alignment);

/* Aligns a given value to a given alignment */
uint32_t hutils_align_value(uint32_t value, uint32_t alignment);

/* Spawns (fork and exec) a new process. Returns, for the parent process, -1
 * in case of error and child's PID (> 0) if success. For the child process,
 * returns -1 in case of error and 0 in case of success */
int hutils_spawn_chld (const char *program, char *const argv[]);

/* Wait for a child process, printing the exit status and possible errors
 * in the global LOG. Returns 0 in case of success and -1 in case of error */
int hutils_wait_chld (void);

/* Wait for a child process with a looped timeout, printing the exit status
 * and possible errors in the global LOG. Returns 0 in case of success and
 * -1 in case of error */
int hutils_wait_chld_timed (int timeout);

/* Clones a str NULL terminated string and return it to the called. Returns NULL
 * in case of error */
char *hutils_clone_str (const char *str);

/* Copies a src NULL terminated string into a dest pre-allocated buffer, up to
 * a maximum of size bytes */
int hutils_copy_str (char *dest, const char *src, size_t size);

/* Get properties from config file (defined in http://rfc.zeromq.org/spec:4)
 * and store them in hash table in the form <property name / property value> */
hutils_err_e hutils_get_hints (zconfig_t *root_cfg, zhashx_t *hints_h);

/* Get board type string from config file */
hutils_err_e hutils_get_board_type (zhashx_t *hints, uint32_t dev_id, 
    char **board_type);

#ifdef __cplusplus
}
#endif

#endif
