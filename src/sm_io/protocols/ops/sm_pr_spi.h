/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_PR_SPI_H_
#define _SM_PR_SPI_H_

#include "sm_pr.h"
#include "sm_pr_err.h"
#include "hw/wb_spi_regs.h"
#include "protocols/smpr_spi_regs.h"

#define SM_PR_SPI_MAX_TRIES                 10
#define SM_PR_SPI_USLEEP                    1000

#define SMPR_PROTO_SPI(self)                ((smpr_proto_spi_t *) self->proto_handler)

/* SPI open arguments */
struct _spi_proto_args_t {
    uint32_t sys_freq;          /* System clock [Hz] */
    uint32_t spi_freq;          /* SPI clock [Hz] */
    uint32_t init_config;       /* SPI initial config register */
};

typedef struct _spi_proto_args_t spi_proto_args_t;

enum _spi_mode_e {
    SPI_MODE_WRITE = 0,
    SPI_MODE_READ,
    SPI_MODE_WRITE_READ
};

typedef enum _spi_mode_e spi_mode_e;

/* For use by llio_t general structure */
extern const smpr_proto_ops_t smpr_proto_ops_spi;

/* Device endpoint */
struct _smpr_proto_spi_t {
    uint64_t base;              /* Core base address */
    uint32_t sys_freq;          /* System clock [Hz] */
    uint32_t spi_freq;          /* SPI clock [Hz] */
    uint32_t init_config;       /* SPI initial config register */
};

/* Opaque smpr_proto_spi structure */
typedef struct _smpr_proto_spi_t smpr_proto_spi_t;

/***************** Our methods *****************/

/* Creates a new instance of the Endpoint*/
smpr_proto_spi_t * smpr_proto_spi_new (uint64_t base);
/* Destroy an instance of the Endpoint */
smpr_err_e smpr_proto_spi_destroy (smpr_proto_spi_t **self_p);

#endif
