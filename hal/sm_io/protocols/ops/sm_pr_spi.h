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
#include "wb_spi_regs.h"

#define SM_PR_SPI_READBACK                  1
#define SM_PR_SPI_MAX_TRIES                 10
#define SM_PR_SPI_USLEEP                    1000

#define SMPR_BYTE_2_BIT                     8
#define SMPR_WB_REG_2_BYTE                  4       /* 32-bit word */
#define SMPR_WB_REG_2_BIT                   (SMPR_WB_REG_2_BYTE*SMPR_BYTE_2_BIT)

#define SMPR_PROTO_SPI(self)                ((smpr_proto_spi_t *) self->proto_handler)

/* Our flags are: SPI_PROTO_CHAR_LEN_MASK | SPI_PROTO_SS_MASK */
#define SMPR_PROTO_SPI_SS_FLAGS_R(reg)      SPI_PROTO_SS_R(reg)
#define SMPR_PROTO_SPI_CHARLEN_FLAGS_R(reg)    \
                                            SPI_PROTO_CTRL_CHAR_LEN_R(reg >> SPI_PROTO_SS_SIZE)

#define SMPR_PROTO_SPI_SS_FLAGS_W(value)    SPI_PROTO_SS_W(value)
#define SMPR_PROTO_SPI_CHARLEN_FLAGS_W(value)    \
                                            SPI_PROTO_CTRL_CHAR_LEN_W(value << SPI_PROTO_SS_SIZE)

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
    uint32_t base;              /* Core base address */
    uint32_t sys_freq;          /* System clock [Hz] */
    uint32_t spi_freq;          /* SPI clock [Hz] */
    uint32_t init_config;       /* SPI initial config register */
};

/* Opaque smpr_proto_spi structure */
typedef struct _smpr_proto_spi_t smpr_proto_spi_t;

/***************** Our methods *****************/

/* Creates a new instance of the Endpoint*/
smpr_proto_spi_t * smpr_proto_spi_new (uint32_t base);
/* Destroy an instance of the Endpoint */
smpr_err_e smpr_proto_spi_destroy (smpr_proto_spi_t **self_p);

#endif
