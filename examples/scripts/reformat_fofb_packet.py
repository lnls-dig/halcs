#!/usr/bin/env python3

import sys
import re

def conv_int32_to_uint32(x):
    INT32_TO_UINT32 = (1 << 32)
    INT32_SIGN_BIT = (1 << 31)
    return x + INT32_TO_UINT32 if (x & INT32_SIGN_BIT) else x

LINE_PATTERN = "[\t ]*(-?\d+)[\t ]*(-?\d+)[\t ]*(-?\d+)[\t ]*(-?\d+)"
p = re.compile(LINE_PATTERN)

try:
    data_format = sys.argv[1]
except IndexError:
    data_format = "uint16"

i = 0
print("| {:8} | {:10} | {:8} | {:6} |  {:^17}  |  {:^17}  | {:10} |".
    format("packet #", "tf_cntr_16", "tf_start", "bpm_id", "bpm_x", "bpm_y", "tf_cntr_32"))
for line in sys.stdin:
    m = re.match(p, line)
    atoms = [int(g) for g in m.groups()]
    tf_cntr_16_lsb = (conv_int32_to_uint32(atoms[3]) & 0xFFFF0000) >> 16
    tf_start = (conv_int32_to_uint32(atoms[3]) & 0x8000) >> 15
    bpm_id = (conv_int32_to_uint32(atoms[3]) & 0x7FFF)
    bpm_pos_x = atoms[2]
    bpm_pos_y = atoms[1]
    tf_cnrt_32 = conv_int32_to_uint32(atoms[0])

    if data_format == "uint16":
        print("| {:<8} | {:<10} | {:<8} | {:<6} | {:<8} | {:<8} | {:<8} | {:<8} | {:10} |".
            format(i, tf_cntr_16_lsb, tf_start, bpm_id,
                (conv_int32_to_uint32(bpm_pos_x) & 0xFFFF0000) >> 16, 
                (conv_int32_to_uint32(bpm_pos_x) & 0xFFFF), 
                (conv_int32_to_uint32(bpm_pos_y) & 0xFFFF0000) >> 16, 
                (conv_int32_to_uint32(bpm_pos_y) & 0xFFFF),
                tf_cnrt_32))
    elif data_format == "int32":
        print("| {:<8} | {:<10} | {:<8} | {:<6} |  {:<16}   |  {:<16}   | {:10} |".
            format(i, tf_cntr_16_lsb, tf_start, bpm_id,
                bpm_pos_x,
                bpm_pos_y,
                tf_cnrt_32))
    i = i + 1
