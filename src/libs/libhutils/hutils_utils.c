/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <stdio.h>
#include <sys/wait.h>       /* waitpid */
#include <errno.h>          /* perror */

#include "hutils.h"
#include "hutils_err.h"
#include "errhand.h"
#include "czmq.h"

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

static char *_hutils_concat_strings_raw (const char *str1, const char* str2,
        const char *str3, bool with_sep, char sep);

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
    uint32_t key_len = hutils_num_to_str_len (key, base) + 1; /* +1 for \0 */
    char *key_c = zmalloc (key_len * sizeof (char));
    ASSERT_ALLOC (key_c, err_key_c_alloc);

    snprintf(key_c, key_len, "%x", key);
    /* DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
            "[hutils:stringify_key] key = %s, key_len = %u\n",
            key_c, key_len); */

    return key_c;

err_key_c_alloc:
    return NULL;
}

char *hutils_stringify_dec_key (uint32_t key)
{
    return hutils_stringify_key (key, 10);
}

char *hutils_stringify_hex_key (uint32_t key)
{
    return hutils_stringify_key (key, 16);
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
    if (str3 != NULL) {
        str = zmalloc (strlen (str1) + strlen (str2) + strlen (str3) +
                num_sep_bytes /* separator length */+ 1 /* \0 */);
        ASSERT_ALLOC(str, err_str3_alloc);

        if (with_sep) {
            sprintf (str, "%s%c%s%s", str1, sep, str2, str3);
        }
        else {
            sprintf (str, "%s%s%s", str1, str2, str3);
        }
    }
    else {
        str = zmalloc (strlen(str1) + strlen(str2) +
                num_sep_bytes /* separator length */+ 1 /* \0 */);
        ASSERT_ALLOC(str, err_str2_alloc);

        if (with_sep) {
            sprintf (str, "%s%c%s", str1, sep, str2);
        }
        else {
            sprintf (str, "%s%s", str1, str2);
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

    return 0;
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
err_str_alloc:
    return NULL;
}

int hutils_copy_str (char *dest, const char *src, size_t size)
{
    assert (dest);
    assert (src);

    int errs = snprintf (dest, size, "%s", src);
    return errs;
}
