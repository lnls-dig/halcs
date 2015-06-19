/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
*/

#ifndef _SM_IO_FMC130M_4CH_CORE_H_
#define _SM_IO_FMC130M_4CH_CORE_H_

#define SMIO_AD9510_HANDLER(smio_handler) ((smch_ad9510_t *) smio_handler->smch_ad9510)
#define SMIO_SI57X_HANDLER(smio_handler) ((smch_si57x_t *) smio_handler->smch_si571)

/* The follosing codes were generated via the following command:
 * > echo FMC130M_4CH_ACTIVE | md5sum | cut -c 1-8
 * > cb04db4d
 * > echo FMC130M_4CH_ACTIVE | md5sum | cut -c 1-8
 * > 59da56ae
 */
#define FMC130M_4CH_ACTIVE_MD5              0xcb04db4d
#define FMC130M_4CH_PASSIVE_MD5             0x59da56ae

/* Start writing on EEPROM address 0x0 */
#define FMC130M_4CH_EEPROM_START_ADDR       0x0

typedef enum {
    TYPE_FMC130M_4CH_UNDEF=0xFF,
    TYPE_FMC130M_4CH_ACTIVE=1,
    TYPE_FMC130M_4CH_PASSIVE
} fmc130m_4ch_type_e;

typedef struct {
    fmc130m_4ch_type_e type;                /* FMC130M_4CH type */
    smch_ad9510_t *smch_ad9510;             /* AD9510 chip handler */
    smch_si57x_t *smch_si571;               /* SI571 chip handler */
    smch_24aa64_t *smch_24aa64;             /* 24AA64 chip handler */
    smch_pca9547_t *smch_pca9547;           /* FPGA I2C Switch */
} smio_fmc130m_4ch_t;

/* FPGA FMC130M delay definitions */

#define DLY_TYPE_DATA                       0x01
#define DLY_TYPE_CLK                        0x02
#define DLY_TYPE_ALL                        (DLY_TYPE_DATA | DLY_TYPE_CLK)

typedef enum {
    DLY_INIT_NO = 0,
    DLY_INIT_YES,
    DLY_INIT_END                            /* Placeholder for end of struct */
} fmc130m_4ch_delay_init_e;

/* Holds the delay value for each channel */

typedef struct {
    fmc130m_4ch_delay_init_e init;
    uint32_t value;
} delay_lines_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_fmc130m_4ch_t * smio_fmc130m_4ch_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_fmc130m_4ch_destroy (smio_fmc130m_4ch_t **self_p);

#endif
