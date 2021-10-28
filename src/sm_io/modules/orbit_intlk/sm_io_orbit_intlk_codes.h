/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_ORBIT_INTLK_CODES_H_
#define _SM_IO_ORBIT_INTLK_CODES_H_

/* Messaging OPCODES */
#define ORBIT_INTLK_OPCODE_TYPE                     uint32_t
#define ORBIT_INTLK_OPCODE_SIZE                     (sizeof (ORBIT_INTLK_OPCODE_TYPE))

#define ORBIT_INTLK_OPCODE_SET_GET_EN               0
#define ORBIT_INTLK_NAME_SET_GET_EN                 "orbit_intlk_set_get_en"
#define ORBIT_INTLK_OPCODE_SET_GET_CLR              1
#define ORBIT_INTLK_NAME_SET_GET_CLR                "orbit_intlk_set_get_clr"
#define ORBIT_INTLK_OPCODE_SET_GET_MIN_SUM_EN       2
#define ORBIT_INTLK_NAME_SET_GET_MIN_SUM_EN         "orbit_intlk_set_get_min_sum_en"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_EN         3
#define ORBIT_INTLK_NAME_SET_GET_TRANS_EN           "orbit_intlk_set_get_trans_en"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_CLR        4
#define ORBIT_INTLK_NAME_SET_GET_TRANS_CLR          "orbit_intlk_set_get_trans_clr"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_EN           5
#define ORBIT_INTLK_NAME_SET_GET_ANG_EN             "orbit_intlk_set_get_ang_en"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_CLR          6
#define ORBIT_INTLK_NAME_SET_GET_ANG_CLR            "orbit_intlk_set_get_ang_clr"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_X   7
#define ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER_X     "orbit_intlk_set_get_trans_bigger_x"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_Y   8
#define ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER_Y     "orbit_intlk_set_get_trans_bigger_y"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_LTC_X  9
#define ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER_LTC_X    "orbit_intlk_set_get_trans_bigger_ltc_x"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_LTC_Y  10
#define ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER_LTC_Y    "orbit_intlk_set_get_trans_bigger_ltc_y"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_ANY    11
#define ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER_ANY      "orbit_intlk_set_get_trans_bigger_any"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER        12
#define ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER          "orbit_intlk_set_get_trans_bigger"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_LTC    13
#define ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER_LTC      "orbit_intlk_set_get_trans_bigger_ltc"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_X        14
#define ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER_X          "orbit_intlk_set_get_ang_bigger_x"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_Y        15
#define ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER_Y          "orbit_intlk_set_get_ang_bigger_y"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_LTC_X    16
#define ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER_LTC_X      "orbit_intlk_set_get_ang_bigger_ltc_x"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_LTC_Y    17
#define ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER_LTC_Y      "orbit_intlk_set_get_ang_bigger_ltc_y"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_ANY      18
#define ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER_ANY        "orbit_intlk_set_get_ang_bigger_any"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER          19
#define ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER            "orbit_intlk_set_get_ang_bigger"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_LTC      20
#define ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER_LTC        "orbit_intlk_set_get_ang_bigger_ltc"
#define ORBIT_INTLK_OPCODE_SET_GET_INTLK               21
#define ORBIT_INTLK_NAME_SET_GET_INTLK                 "orbit_intlk_set_get_intlk"
#define ORBIT_INTLK_OPCODE_SET_GET_INTLK_LTC           22
#define ORBIT_INTLK_NAME_SET_GET_INTLK_LTC             "orbit_intlk_set_get_intlk_ltc"
#define ORBIT_INTLK_OPCODE_SET_GET_MIN_SUM             23
#define ORBIT_INTLK_NAME_SET_GET_MIN_SUM               "orbit_intlk_set_get_min_sum"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_MAX_X         24
#define ORBIT_INTLK_NAME_SET_GET_TRANS_MAX_X           "orbit_intlk_set_get_trans_max_x"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_MAX_Y         25
#define ORBIT_INTLK_NAME_SET_GET_TRANS_MAX_Y           "orbit_intlk_set_get_trans_max_y"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_MAX_X           26
#define ORBIT_INTLK_NAME_SET_GET_ANG_MAX_X             "orbit_intlk_set_get_ang_max_x"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_MAX_Y           27
#define ORBIT_INTLK_NAME_SET_GET_ANG_MAX_Y             "orbit_intlk_set_get_ang_max_y"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER_X     28
#define ORBIT_INTLK_NAME_SET_GET_TRANS_SMALLER_X       "orbit_intlk_set_get_trans_smaller_x"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER_Y     29
#define ORBIT_INTLK_NAME_SET_GET_TRANS_SMALLER_Y       "orbit_intlk_set_get_trans_smaller_y"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER_LTC_X  30
#define ORBIT_INTLK_NAME_SET_GET_TRANS_SMALLER_LTC_X    "orbit_intlk_set_get_trans_smaller_ltc_x"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER_LTC_Y  31
#define ORBIT_INTLK_NAME_SET_GET_TRANS_SMALLER_LTC_Y    "orbit_intlk_set_get_trans_smaller_ltc_y"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER_ANY    32
#define ORBIT_INTLK_NAME_SET_GET_TRANS_SMALLER_ANY      "orbit_intlk_set_get_trans_smaller_any"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER        33
#define ORBIT_INTLK_NAME_SET_GET_TRANS_SMALLER          "orbit_intlk_set_get_trans_smaller"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER_LTC    34
#define ORBIT_INTLK_NAME_SET_GET_TRANS_SMALLER_LTC      "orbit_intlk_set_get_trans_smaller_ltc"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER_X        35
#define ORBIT_INTLK_NAME_SET_GET_ANG_SMALLER_X          "orbit_intlk_set_get_ang_smaller_x"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER_Y        36
#define ORBIT_INTLK_NAME_SET_GET_ANG_SMALLER_Y          "orbit_intlk_set_get_ang_smaller_y"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER_LTC_X    37
#define ORBIT_INTLK_NAME_SET_GET_ANG_SMALLER_LTC_X      "orbit_intlk_set_get_ang_smaller_ltc_x"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER_LTC_Y    38
#define ORBIT_INTLK_NAME_SET_GET_ANG_SMALLER_LTC_Y      "orbit_intlk_set_get_ang_smaller_ltc_y"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER_ANY      39
#define ORBIT_INTLK_NAME_SET_GET_ANG_SMALLER_ANY        "orbit_intlk_set_get_ang_smaller_any"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER          40
#define ORBIT_INTLK_NAME_SET_GET_ANG_SMALLER            "orbit_intlk_set_get_ang_smaller"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER_LTC      41
#define ORBIT_INTLK_NAME_SET_GET_ANG_SMALLER_LTC        "orbit_intlk_set_get_ang_smaller_ltc"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_MIN_X          42
#define ORBIT_INTLK_NAME_SET_GET_TRANS_MIN_X            "orbit_intlk_set_get_trans_min_x"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_MIN_Y          43
#define ORBIT_INTLK_NAME_SET_GET_TRANS_MIN_Y            "orbit_intlk_set_get_trans_min_y"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_MIN_X            44
#define ORBIT_INTLK_NAME_SET_GET_ANG_MIN_X              "orbit_intlk_set_get_ang_min_x"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_MIN_Y            45
#define ORBIT_INTLK_NAME_SET_GET_ANG_MIN_Y              "orbit_intlk_set_get_ang_min_y"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_X_DIFF         46
#define ORBIT_INTLK_NAME_SET_GET_TRANS_X_DIFF           "orbit_intlk_set_get_trans_x_diff"
#define ORBIT_INTLK_OPCODE_SET_GET_TRANS_Y_DIFF         47
#define ORBIT_INTLK_NAME_SET_GET_TRANS_Y_DIFF           "orbit_intlk_set_get_trans_y_diff"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_X_DIFF           48
#define ORBIT_INTLK_NAME_SET_GET_ANG_X_DIFF             "orbit_intlk_set_get_ang_x_diff"
#define ORBIT_INTLK_OPCODE_SET_GET_ANG_Y_DIFF           49
#define ORBIT_INTLK_NAME_SET_GET_ANG_Y_DIFF             "orbit_intlk_set_get_ang_y_diff"
#define ORBIT_INTLK_OPCODE_END                          50

/* For compatibility with older API. Don't remove these without changing libclient on clients */
#define ORBIT_INTLK_OPCODE_SET_GET_INTLK_BIGGER         ORBIT_INTLK_OPCODE_SET_GET_INTLK
#define ORBIT_INTLK_NAME_SET_GET_INTLK_BIGGER           ORBIT_INTLK_NAME_SET_GET_INTLK
#define ORBIT_INTLK_OPCODE_SET_GET_INTLK_BIGGER_LTC     ORBIT_INTLK_OPCODE_SET_GET_INTLK_LTC
#define ORBIT_INTLK_NAME_SET_GET_INTLK_BIGGER_LTC       ORBIT_INTLK_NAME_SET_GET_INTLK_LTC

/* Messaging Reply OPCODES */
#define ORBIT_INTLK_REPLY_TYPE                      uint32_t
#define ORBIT_INTLK_REPLY_SIZE                      (sizeof (ORBIT_INTLK_REPLY_TYPE))

#define ORBIT_INTLK_OK                              0   /* Operation was successful */
#define ORBIT_INTLK_ERR                             1   /* Generic error */

#endif
