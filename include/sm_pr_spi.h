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
} spi_proto_args_t;

typedef enum {
    SPI_MODE_WRITE = 0,
    SPI_MODE_READ,
    SPI_MODE_WRITE_READ
} spi_mode_e;

/* For use by llio_t general structure */
extern const smpr_proto_ops_t smpr_proto_ops_spi;

#ifdef __cplusplus
}
#endif

#endif
