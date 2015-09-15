#!/usr/bin/env bash

# help debug
set -x

# Build and local install repositories
mkdir tmp
BUILD_PREFIX=$PWD/tmp

CONFIG_FLAGS=()
CONFIG_FLAGS+=("CFLAGS=-I${BUILD_PREFIX}/include")
CONFIG_FLAGS+=("CPPFLAGS=-I${BUILD_PREFIX}/include")
CONFIG_FLAGS+=("CXXFLAGS=-I${BUILD_PREFIX}/include")
CONFIG_FLAGS+=("LDFLAGS=-L${BUILD_PREFIX}/lib")
CONFIG_OPTS=($CONFIG_FLAGS)
CONFIG_OPTS+=("PKG_CONFIG_PATH=${BUILD_PREFIX}/lib/pkgconfig")
CONFIG_OPTS+=("--prefix=${BUILD_PREFIX}")

#   libsodium
git clone git://github.com/jedisct1/libsodium.git &&
( cd libsodium; ./autogen.sh && ./configure --prefix=$BUILD_PREFIX &&
    make check && make install ) || exit 1

#   libzmq
git clone git://github.com/zeromq/libzmq.git &&
( cd libzmq; ./autogen.sh && ./configure  "${CONFIG_OPTS[@]}" &&
    make check && make install ) || exit 1

#   CZMQ
git clone git://github.com/zeromq/czmq.git &&
( cd czmq; ./autogen.sh && ./configure  "${CONFIG_OPTS[@]}" &&
    make check && make install ) || exit 1

#   Zyre
git clone git://github.com/zeromq/zyre.git &&
( cd zyre; ./autogen.sh && ./configure  "${CONFIG_OPTS[@]}" &&
    make check && make install ) || exit 1

#   Malamute
git clone git://github.com/zeromq/malamute.git &&
( cd malamute; ./autogen.sh && ./configure  "${CONFIG_OPTS[@]}" &&
    make check && make install ) || exit 1

./compile.sh $BOARD $WITH_EXAMPLES "${CONFIG_FLAGS[@]}"
