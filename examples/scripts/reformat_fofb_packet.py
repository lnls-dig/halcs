#!/usr/bin/env python3

import sys
import re

def conv_int32_to_uint32(x):
    INT32_TO_UINT32 = (1 << 32)
    INT32_SIGN_BIT = (1 << 31)
    return x + INT32_TO_UINT32 if (x & INT32_SIGN_BIT) else x

LINE_PATTERN = "[\t ]*(-?\d+)[\t ]*(-?\d+)[\t ]*(-?\d+)[\t ]*(-?\d+)"
p = re.compile(LINE_PATTERN)

i = 0
print("| {:8} | {:10} | {:8} | {:6} |  {:^17}  |  {:^17}  | {:10} |".
    format("packet #", "tf_cntr_16", "tf_start", "bpm_id", "bpm_x", "bpm_y", "tf_cntr_32"))
for line in sys.stdin:
    m = re.match(p, line)
    atoms = [conv_int32_to_uint32(int(g)) for g in m.groups()]
    tf_cntr_16_lsb = (atoms[3] & 0xFFFF0000) >> 16
    tf_start = (atoms[3] & 0x8000) >> 15
    bpm_id = (atoms[3] & 0x7FFF)
    bpm_pos_x_16_msb = (atoms[2] & 0xFFFF0000) >> 16
    bpm_pos_x_16_lsb = (atoms[2] & 0xFFFF)
    bpm_pos_y_16_msb = (atoms[1] & 0xFFFF0000) >> 16
    bpm_pos_y_16_lsb = (atoms[1] & 0xFFFF)
    tf_cnrt_32 = atoms[0]
    print("| {:<8} | {:<10} | {:<8} | {:<6} | {:<8} | {:<8} | {:<8} | {:<8} | {:10} |".
        format(i, tf_cntr_16_lsb, tf_start, bpm_id,
            bpm_pos_x_16_msb, bpm_pos_x_16_lsb, 
            bpm_pos_y_16_msb, bpm_pos_y_16_lsb,
            tf_cnrt_32))
    i = i + 1
