#!/usr/bin/env bash

# help debug
set -x

# Build and local install repositories
mkdir tmp
mkdir tmp/etc
BUILD_PREFIX=$PWD/tmp
SCRIPTS_PREFIX=$PWD/tmp/etc

LIBSODIUM_VER=1.0.3
LIBZMQ_VER=v4.2.0
LIBCZMQ_VER=v4.0.1
MALAMUTE_VER=v1.3
ZYRE_VER=v1.1.0

CONFIG_FLAGS=()
CONFIG_FLAGS+=("CFLAGS=-I${BUILD_PREFIX}/include")
CONFIG_FLAGS+=("CPPFLAGS=-I${BUILD_PREFIX}/include")
CONFIG_FLAGS+=("CXXFLAGS=-I${BUILD_PREFIX}/include")
CONFIG_FLAGS+=("LDFLAGS=-L${BUILD_PREFIX}/lib")

KERNEL_FLAGS=()
KERNEL_FLAGS+=("INSTALLDIR=${BUILD_PREFIX}/lib/modules/$(shell uname -r)/extra")
KERNEL_FLAGS+=("INSTALLHDRDIR=${BUILD_PREFIX}/include/pciDriver/driver")

HALCS_OPTS=()
HALCS_OPTS+=(${CONFIG_FLAGS[@]})
HALCS_OPTS+=(${KERNEL_FLAGS[@]})
HALCS_OPTS+=("PREFIX=${BUILD_PREFIX}")
HALCS_OPTS+=("SCRIPTS_PREFIX=${SCRIPTS_PREFIX}")

CONFIG_OPTS=()
CONFIG_OPTS+=(${CONFIG_FLAGS[@]})
CONFIG_OPTS+=("PKG_CONFIG_PATH=${BUILD_PREFIX}/lib/pkgconfig")
CONFIG_OPTS+=("--prefix=${BUILD_PREFIX}")

#   libsodium
git clone --branch=${LIBSODIUM_VER} git://github.com/jedisct1/libsodium.git &&
( cd libsodium; ./autogen.sh && ./configure --prefix=$BUILD_PREFIX &&
    make check && make install ) || exit 1

#   libzmq
git clone --branch=${LIBZMQ_VER} git://github.com/zeromq/libzmq.git &&
( cd libzmq; ./autogen.sh && ./configure  "${CONFIG_OPTS[@]}" &&
    make check && make install ) || exit 1

#   CZMQ
git clone --branch=${LIBCZMQ_VER} git://github.com/zeromq/czmq.git &&
( cd czmq; ./autogen.sh && ./configure  "${CONFIG_OPTS[@]}" &&
    make check && make install ) || exit 1

#   Zyre
git clone --branch=${ZYRE_VER} git://github.com/zeromq/zyre.git &&
( cd zyre; ./autogen.sh && ./configure  "${CONFIG_OPTS[@]}" &&
    make check && make install ) || exit 1

#   Malamute
git clone --branch=${MALAMUTE_VER} git://github.com/lnls-dig/malamute.git &&
( cd malamute; ./autogen.sh && ./configure  "${CONFIG_OPTS[@]}" &&
    make check && make install ) || exit 1

./compile.sh -b $BOARD -a "${APP}" -e $EXAMPLES -l $SYSTEM_INTEGRATION -x "${HALCS_OPTS[*]}"
