/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "hutils.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, HAL_UTILS, "[hutils:utils]",          \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)           \
    ASSERT_HAL_ALLOC(ptr, HAL_UTILS, "[hutils:utils]",                  \
            hutils_err_str(HUTILS_ERR_ALLOC),                           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                        \
    CHECK_HAL_ERR(err, HAL_UTILS, "[hutils:utils]",                     \
            hutils_err_str (err_type))

#define MIN_WAIT_TIME           1                           /* in ms */
#define MSECS                   1000                        /* in seconds */

static char *_hutils_concat_strings_raw (const char *str1, const char* str2,
        const char *str3, bool with_sep, char sep);
static void _hutils_hints_free_item (void **data);

/*******************************************************************/
/*****************  String manipulation functions ******************/
/*******************************************************************/

uint32_t hutils_num_to_str_len (uint32_t key, uint32_t base)
{
    uint32_t i = 0;
    uint32_t rem = key;

    do {
        rem /= base;
        ++i;
    } while (rem > 0);

    return i;
}

uint32_t hutils_hex_to_str_len (uint32_t key)
{
    return hutils_num_to_str_len (key, 16);
}

uint32_t hutils_dec_to_str_len (uint32_t key)
{
    return hutils_num_to_str_len (key, 10);
}

char *hutils_stringify_key (uint32_t key, uint32_t base)
{
    char *key_c = NULL;

    switch (base) {
        case 10:
            key_c = hutils_stringify_dec_key (key);
        break;

        case 16:
            key_c = hutils_stringify_hex_key (key);
        break;

        default:
            DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_ERR,
                    "[hutils:stringify_key] invalid base = %u\n",
                    base);
            goto err_inv_base;
    }

    return key_c;

err_inv_base:
    return key_c;
}

char *hutils_stringify_dec_key (uint32_t key)
{
    size_t key_len = hutils_num_to_str_len (key, 10) + 1; /* +1 for \0 */
    char *key_c = zmalloc (key_len);
    ASSERT_ALLOC (key_c, err_key_c_alloc);

    snprintf(key_c, key_len, "%u", key);

    return key_c;

err_key_c_alloc:
    return key_c;
}

char *hutils_stringify_hex_key (uint32_t key)
{
    size_t size = sizeof (key);
    uint8_t *data = (uint8_t *) &key;
    char *key_c = zmalloc (size * 2 + 1);
    ASSERT_ALLOC (key_c, err_key_c_alloc);

    static const char hex_char [] = "0123456789ABCDEF";
    uint32_t i = 0;
    for (i = 0; i < size; ++i) {
        key_c [i * 2 + 0] = hex_char [data [size-1-i] >> 4];
        key_c [i * 2 + 1] = hex_char [data [size-1-i] & 15];
    }
    key_c [size * 2] = 0;

    return key_c;

err_key_c_alloc:
    return key_c;
}

uint32_t hutils_numerify_key (const char *key, uint32_t base)
{
    return strtoul (key, NULL, base);
}

uint32_t hutils_numerify_dec_key (const char *key)
{
    return hutils_numerify_key (key, 10);
}

uint32_t hutils_numerify_hex_key (const char *key)
{
    return hutils_numerify_key (key, 16);
}

#define SEPARATOR_BYTES 1
/* FIXME: poorly written */
static char *_hutils_concat_strings_raw (const char *str1, const char* str2,
        const char *str3, bool with_sep, char sep)
{
    assert (str1);
    assert (str2);

    uint32_t num_sep_bytes = (with_sep)? SEPARATOR_BYTES : 0;

    char *str = NULL;
    size_t str_len = 0;
    if (str3 != NULL) {
        str_len = strlen (str1) + strlen (str2) + strlen (str3) +
            num_sep_bytes /* separator length */ + 1 /* \0 */;
        str = zmalloc (str_len);
        ASSERT_ALLOC(str, err_str3_alloc);

        if (with_sep) {
            snprintf (str, str_len, "%s%c%s%s", str1, sep, str2, str3);
        }
        else {
            snprintf (str, str_len, "%s%s%s", str1, str2, str3);
        }
    }
    else {
        str_len = strlen(str1) + strlen(str2) + 
            num_sep_bytes /* separator length */ + 1 /* \0 */;
        str = zmalloc (str_len);
        ASSERT_ALLOC(str, err_str2_alloc);

        if (with_sep) {
            snprintf (str, str_len, "%s%c%s", str1, sep, str2);
        }
        else {
            snprintf (str, str_len, "%s%s", str1, str2);
        }
    }

    return str;

err_str3_alloc:
err_str2_alloc:
    return NULL;
}

char *hutils_concat_strings (const char *str1, const char* str2, char sep)
{
    return _hutils_concat_strings_raw (str1, str2, NULL, true, sep);
}

char *hutils_concat_strings_no_sep (const char *str1, const char* str2)
{
    return _hutils_concat_strings_raw (str1, str2, NULL, false, 0);
}

char *hutils_concat_strings3 (const char *str1, const char* str2,
        const char* str3, char sep)
{
    return _hutils_concat_strings_raw (str1, str2, str3, true, sep);
}

char *hutils_concat_strings3_no_sep (const char *str1, const char* str2,
        const char* str3)
{
    return _hutils_concat_strings_raw (str1, str2, str3, false, 0);
}

/*******************************************************************/
/*****************  Byte manipulation functions ********************/
/*******************************************************************/

uint32_t hutils_calculate_padding(uint32_t value, uint32_t alignment)
{
    uint32_t exceeded = value % alignment;
    uint32_t remaining = alignment - exceeded;
    uint32_t padding = remaining % alignment;

    return padding;
}

uint32_t hutils_align_value(uint32_t value, uint32_t alignment)
{
    return value + hutils_calculate_padding(value, alignment);
}

/*******************************************************************/
/*****************  System Fork/Exec functions *********************/
/*******************************************************************/

int hutils_spawn_chld (const char *program, char *const argv[])
{
    pid_t child = fork ();

    if (child == -1) {
        DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_FATAL, "[hutils:utils] Could not fork child. "
                "Errno = %d, %s\n", errno, strerror(errno));
        /* What to do in case of error? retry ? */
        return -1;
    }
    else if (child == 0) { /* Child */
        int err = execv (program, argv);

        if (err < 0) {
            DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_FATAL, "[hutils:utils] Could not exec child. "
                    "Errno = %d, %s\n", errno, strerror(errno));
            return -1;
        }
    }
    else { /* Parent */
    }

    return child;
}

int hutils_wait_chld (void)
{
    int chld_status;
    pid_t chld_pid = waitpid (-1, &chld_status, WNOHANG);

    /* Error or no child exists */
    if (chld_pid == (pid_t) -1) {
        /* Not actually an error if ECHILD. Do nothing... */
        if (errno == ECHILD) {
            /* DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_INFO, "[hutils:utils] no child to wait for\n"); */
            return 0;
        }

        return -1;
    }

    /* Child exists but have not changed its state */
    if (chld_pid == (pid_t) 0) {
        /* DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_INFO, "[hutils:utils] Child has not changed its state\n"); */
        return 0;
    }

    /* Child exists and has changed its state. Check fior the return status */
    if (WIFEXITED (chld_status)) {
        DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_WARN, "[hutils:utils] Child exited%s with status %d\n",
                WCOREDUMP(chld_status) ? " and dumped core" : "",
                WEXITSTATUS(chld_status));
    }

    if (WIFSTOPPED (chld_status)) {
        DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_WARN, "[hutils:utils] Child stopped by signal %d\n",
                WSTOPSIG(chld_status));
    }

    if (WIFSIGNALED (chld_status)) {
        DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_WARN, "[hutils:utils] Child signalled by signal %d\n",
                WTERMSIG(chld_status));
    }

    return chld_pid;
}

int hutils_wait_chld_timed (int timeout)
{
    int err = 0;

    time_t start = time (NULL);
    while ((time(NULL) - start)*1000 < timeout) {
        if (zsys_interrupted) {
            err = -1;
            goto halcs_zsys_interrupted;
        }

        err = hutils_wait_chld ();
        if (err > 0) {
            DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_WARN, "[hutils:utils] "
                    "hutils_wait_chld_timed: finished waiting\n");
            goto exit;
        }

        usleep (MSECS*MIN_WAIT_TIME);
    }

exit:
halcs_zsys_interrupted:
    return err;
}

/*******************************************************************/
/************************ Revision functions ***********************/
/*******************************************************************/

char *hutils_clone_str (const char *str)
{
    assert (str);

    size_t str_size = strlen (str)+1;
    char *new_str = zmalloc (str_size);
    ASSERT_ALLOC (new_str, err_str_alloc);

    int errs = snprintf (new_str, str_size, "%s", str);
    ASSERT_TEST (errs >= 0,
            "[hutils:utils] Could not clone string. Enconding error?\n",
            err_copy_str);
    /* This shouldn't happen. Only when the number of characters written is
     * less than the whole buffer, it is guaranteed that the string was
     * written successfully */
    ASSERT_TEST ((size_t) errs < str_size,
            "[hutils:utils] Cloned string was truncated\n", err_trunc_str);

    return new_str;

err_trunc_str:
err_copy_str:
    free (new_str);
err_str_alloc:
    return NULL;
}

int hutils_copy_str (char *dest, const char *src, size_t size)
{
    assert (dest);
    assert (src);

    int errs = snprintf (dest, size, "%s", src);

    ASSERT_TEST (errs >= 0,
            "[hutils:utils] Could not clone string. Enconding error?\n",
            err_copy_str);
    /* This shouldn't happen. Only when the number of characters written is
     * less than the whole buffer, it is guaranteed that the string was
     * written successfully */
    ASSERT_TEST ((size_t) errs < size,
            "[hutils:utils] Cloned string was truncated\n", err_trunc_str);

err_trunc_str:
    return errs+1 /* \0 */;
err_copy_str:
    return errs;
}

/*******************************************************************/
/********************* ZCONFIG read functions  *********************/
/*******************************************************************/

/* Hash free function */
static void _hutils_hints_free_item (void **data)
{
    assert (data);

    if (*data) {
        hutils_hints_t *item = *data;
        free (item->board_type);
        item->board_type = NULL;
        free (item->bind);
        item->bind = NULL;
        free (item->proto);
        item->proto = NULL;
        free (item);
        *data = NULL;
    }
}

/* Get properties from config file (defined in http://rfc.zeromq.org/spec:4)
 * and store them in hash table in the form <property name / property value> */
hutils_err_e hutils_get_hints (zconfig_t *root_cfg, zhashx_t *hints_h)
{
    assert (root_cfg);
    assert (hints_h);

    hutils_err_e err = HUTILS_SUCCESS;
    hutils_hints_t *item = NULL;

    /* Set destruction function */
    zhashx_set_destructor (hints_h, _hutils_hints_free_item);

    /* Read DEVIO suggested bind endpoints and fill the hash table with
     * the corresponding keys */

    /* First find the dev_io property */
    zconfig_t *hutils_cfg = zconfig_locate (root_cfg, "/dev_io");
    ASSERT_TEST (hutils_cfg != NULL, "Could not find "
            "dev_io property in configuration file", err_cfg_exit,
            HUTILS_ERR_CFG);

    /* Now, find all of our child */
    zconfig_t *board_cfg = zconfig_child (hutils_cfg);
    ASSERT_TEST (board_cfg != NULL, "Could not find "
            "board* property in configuration file", err_cfg_exit,
            HUTILS_ERR_CFG);

    /* Navigate through all of our board siblings */
    for (; board_cfg != NULL; board_cfg = zconfig_next (board_cfg)) {
        DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE, "Config file: "
                "board_cfg name: %s\n", zconfig_name (board_cfg));

        zconfig_t *halcs_cfg = zconfig_child (board_cfg);
        ASSERT_TEST (halcs_cfg != NULL, "Could not find "
                "halcs* property in configuration file", err_cfg_exit,
                HUTILS_ERR_CFG);

        /* Navigate through all of our halcs siblings and fill the hash table */
        for (; halcs_cfg != NULL; halcs_cfg = zconfig_next (halcs_cfg)) {
            DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE, "Config file: "
                    "halcs_cfg name: %s\n", zconfig_name (halcs_cfg));

            /* Create hash item */
            item = (hutils_hints_t *) zmalloc (sizeof *item);
            ASSERT_ALLOC(item, err_hash_item_alloc, HUTILS_ERR_ALLOC);

            /* We expect a DBE board type of this halcs/board instance
             * in the configuration file */
            char *board_type = zconfig_resolve (halcs_cfg, "/dbe/board_type",
                    NULL);
            ASSERT_TEST (board_type != NULL, "[hutils:utils] Could not find "
                    "DBE Board type (board_type = <value>) in configuration file", err_board_type,
                    HUTILS_ERR_CFG);

            item->board_type = strdup (board_type);
            ASSERT_ALLOC(item->board_type, err_hash_board_type, HUTILS_ERR_ALLOC);

            /* Now, we expect to find the bind address of this halcs/board instance
             * in the configuration file */
            char *afe_bind = zconfig_resolve (halcs_cfg, "/afe/bind",
                    NULL);
            ASSERT_TEST (afe_bind != NULL, "[hutils:utils] Could not find "
                    "AFE bind address (bind = <value>) in configuration file", err_afe_bind,
                    HUTILS_ERR_CFG);

            item->bind = strdup (afe_bind);
            ASSERT_ALLOC(item->bind, err_hash_bind_alloc, HUTILS_ERR_ALLOC);

            /* We expect to find the rffe protocol of this halcs/board instance
             * in the configuration file */
            char *afe_proto = zconfig_resolve (halcs_cfg, "/afe/proto",
                    NULL);

            if (afe_proto == NULL) {
                item->proto = strdup ("bsmp");
            } else {
                item->proto = strdup (afe_proto);
            }

            ASSERT_ALLOC(item->proto, err_hash_proto_alloc, HUTILS_ERR_ALLOC);

            /* Now, we only need to generate a valid key to insert in the hash.
             * we choose the combination of the type "board%u/halcs%u/afe" or
             * board%u/halcs%u/dbe */
            char hints_key [HUTILS_CFG_HASH_KEY_MAX_LEN];
            int errs = snprintf (hints_key, sizeof (hints_key),
                    HUTILS_CFG_HASH_KEY_PATTERN, zconfig_name (board_cfg),
                    zconfig_name (halcs_cfg));

            /* Only when the number of characters written is less than the whole buffer,
             * it is guaranteed that the string was written successfully */
            ASSERT_TEST (errs >= 0 && (size_t) errs < sizeof (hints_key),
                    "[hutils:utils] Could not generate AFE bind address from "
                    "configuration file\n", err_gen_hash_key, HUTILS_ERR_CFG);

            DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_INFO, "[hutils:utils] CFG hints "
                    "hash key: \"%s\", board_type: \"%s\", bind: \"%s\"\n",
                    hints_key, board_type, afe_bind);

            /* Insert this value in the hash table */
            errs = zhashx_insert (hints_h, hints_key, item);
            ASSERT_TEST (errs == 0, "Could not insert CFG item to hints "
                    "hash table", err_hash_insert, HUTILS_ERR_CFG);
        }
    }

    return err;

    /* Free only the last item on error. The other ones will be freed by the hash table,
     * on destruction */
err_hash_insert:
err_gen_hash_key:
    free (item->proto);
err_hash_proto_alloc:
    free (item->bind);
err_hash_bind_alloc:
err_afe_bind:
    free (item->board_type);
err_hash_board_type:
err_board_type:
    free (item);
err_hash_item_alloc:
err_cfg_exit:
    return err;
}

hutils_err_e hutils_get_board_type (zhashx_t *hints, uint32_t dev_id, 
    char **board_type)
{
    assert (hints);
    hutils_err_e err = HUTILS_SUCCESS;

    char hints_key [HUTILS_CFG_HASH_KEY_MAX_LEN];
    int errs = snprintf (hints_key, sizeof (hints_key),
            HUTILS_CFG_HASH_KEY_PATTERN_COMPL, dev_id, 0);

    /* Only when the number of characters written is less than the whole buffer,
     * it is guaranteed that the string was written successfully */
    ASSERT_TEST (errs >= 0 && (size_t) errs < sizeof (hints_key),
            "Could not generate configuration hash key for configuration "
            "file", err_cfg_exit, HUTILS_ERR_CFG);

    hutils_hints_t *cfg_item = zhashx_lookup (hints, hints_key);
    /* If key is not found, assume we don't have any more AFE to
     * prepare */
    ASSERT_TEST (cfg_item != NULL && cfg_item->board_type != NULL && !streq (cfg_item->board_type, ""),
            "Board type not specified in config file", err_cfg_exit, HUTILS_ERR_CFG);

    *board_type = strdup (cfg_item->board_type);
    ASSERT_ALLOC (*board_type, err_board_type_entry, HUTILS_ERR_ALLOC);

err_board_type_entry:
err_cfg_exit:
    return err;
}
