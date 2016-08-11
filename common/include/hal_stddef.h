/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* Cut from Linux sources */

#ifndef _HAL_STDDEF_H_
#define _HAL_STDDEF_H_

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(type,member) __compiler_offsetof(type, member)
#else
#define offsetof(type, member) ((size_t) &((type *)0)->member)
#endif

/*
* swap - swap value of @a and @b
*/
#define swap(a, b) \
        do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)

/*
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define ARRAY_SIZE(ARRAY) (sizeof(ARRAY)/sizeof((ARRAY)[0]))

/* Signal handler function pointer */
typedef void (*sig_handler_fp)(int sig, siginfo_t *siginfo, void *context);
/* Wait child handler function pointer */
typedef int (*wait_chld_handler_fp)(void);
/* Wait child with timeout handler function pointer */
typedef int (*wait_chld_timed_handler_fp)(int timeout);
/* Spawn child handler function pointer */
typedef int (*spawn_chld_handler_fp)(const char *program, char *const argv[]);
/* Spawn broker handler function pointer */
typedef int (*spawn_broker_handler_fp)(const char *program, char *const argv[]);

#endif
