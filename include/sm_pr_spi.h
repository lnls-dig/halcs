/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_PR_SPI_H_
#define _SM_PR_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

/* SPI open arguments */
typedef struct {
    uint32_t sys_freq;          /* System clock [Hz] */
    uint32_t spi_freq;          /* SPI clock [Hz] */
    uint32_t init_config;       /* SPI initial config register */
    bool bidir;                 /* SPI bidirectional control enable */
} spi_proto_args_t;

typedef enum {
    SPI_MODE_WRITE = 0,
    SPI_MODE_READ,
    SPI_MODE_WRITE_READ
} spi_mode_e;

/************************* Our Methods **************************/

/* Creates a new instance of the proto_spi */
smpr_spi_t *smpr_spi_new (uint32_t ss, uint32_t charlen);
/* Destroy an instance of the spi */
smpr_err_e smpr_spi_destroy (smpr_spi_t **self_p);

/* Set SPI SS parameter */
smpr_err_e smpr_spi_set_ss (smpr_spi_t *self, uint32_t ss);
/* Get SPI SS parameter */
uint32_t smpr_spi_get_ss (smpr_spi_t *self);

/* Set SPI CHARLEN parameter */
smpr_err_e smpr_spi_set_charlen (smpr_spi_t *self, uint32_t charlen);
/* Get SPI CHARLEN parameter */
uint32_t smpr_spi_get_charlen (smpr_spi_t *self);

/* Get SPI operations */
const smpr_proto_ops_t *smpr_spi_get_ops (smpr_spi_t *self);

#ifdef __cplusplus
}
#endif

#endif
