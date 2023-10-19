#!/bin/sh
set -xe

CC="clang"
CFLAGS="-Wall -Wextra -Wpedantic -I.."

BUILD_DIR="build"

if [ ! -d $BUILD_DIR ]; then
    mkdir $BUILD_DIR
fi

$CC $CFLAGS -o $BUILD_DIR/string_view_test string_view_test.c
$CC $CFLAGS -o $BUILD_DIR/arena_libc_backend_test arena_libc_backend_test.c
$CC $CFLAGS -o $BUILD_DIR/common_test common_test.c
