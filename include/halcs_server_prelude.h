/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* Copied from CZMQ repository, available at
 * https://github.com/zeromq/czmq/blob/master/include/czmq_prelude.h */

#ifndef _HALCS_SERVER_PRELUDE_H_
#define _HALCS_SERVER_PRELUDE_H_

/* External dependencies */
#include <inttypes.h>
#include <sys/types.h>
#include <stdbool.h>
#include <getopt.h>
#include <signal.h>

/* zeroMQ libraries */
#include <zmq.h>
#include <czmq.h>
#include <malamute.h>

/* Common macros */
#ifndef UNUSED
#define UNUSED(x) (void)x;
#endif

/* Check for library versions */

/* ZMQ */
#if ZMQ_VERSION < 40200
#error "HALCS requires at least libzmq/4.2.0."
#endif

/* CZMQ */
#if CZMQ_VERSION < 40002
#error "HALCS requires at least czmq/4.0.2"
#endif

/* MLM */
#if MLM_VERSION < 10000
#error "HALCS requires at least malamute/1.0.0"
#endif

#endif
