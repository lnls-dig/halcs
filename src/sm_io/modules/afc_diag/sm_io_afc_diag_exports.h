/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_AFC_DIAG_EXPORTS_H_
#define _SM_IO_AFC_DIAG_EXPORTS_H_

extern disp_op_t afc_diag_set_get_card_slot_exp;
extern disp_op_t afc_diag_set_get_ipmi_addr_exp;
extern disp_op_t afc_diag_get_build_revision_exp;
extern disp_op_t afc_diag_get_build_date_exp;
extern disp_op_t afc_diag_get_build_user_name_exp;
extern disp_op_t afc_diag_get_build_user_email_exp;

extern const disp_op_t *afc_diag_exp_ops [];

#endif

