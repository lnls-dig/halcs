/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 *
 * This was taken from wrpc-sw reposytory at
 * http://www.ohwr.org/projects/wrpc-sw/repository
 */

#ifndef _SDB_H_
#define _SDB_H_

#include <inttypes.h>

#define SDB_INTERCONNET             0x00
#define SDB_DEVICE                  0x01
#define SDB_BRIDGE                  0x02
#define SDB_EMPTY                   0xFF

typedef struct pair64 {
    uint32_t high;
    uint32_t low;
} pair64_t;

struct sdb_empty {
    int8_t reserved[63];
    uint8_t record_type;
};

struct sdb_product {
    pair64_t vendor_id;
    uint32_t device_id;
    uint32_t version;
    uint32_t date;
    int8_t name[19];
    uint8_t record_type;
};

struct sdb_component {
    pair64_t addr_first;
    pair64_t addr_last;
    struct sdb_product product;
};

struct sdb_device {
    uint16_t abi_class;
    uint8_t abi_ver_major;
    uint8_t abi_ver_minor;
    uint32_t bus_specific;
    struct sdb_component sdb_component;
};

struct sdb_bridge {
    pair64_t sdb_child;
    struct sdb_component sdb_component;
};

struct sdb_interconnect {
    uint32_t sdb_magic;
    uint16_t sdb_records;
    uint8_t sdb_version;
    uint8_t sdb_bus_type;
    struct sdb_component sdb_component;
};

typedef union sdb_record {
    struct sdb_empty empty;
    struct sdb_device device;
    struct sdb_bridge bridge;
    struct sdb_interconnect interconnect;
} sdb_record_t;

#endif
