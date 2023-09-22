set -xe

CC="clang"
CFLAGS="-Wall -Wextra -Wpedantic -I.."

BUILD_DIR="build"

if [ ! -d $BUILD_DIR ]; then
    mkdir $BUILD_DIR
fi

$CC $CFLAGS -o $BUILD_DIR/string_view_test.exe string_view_test.c
$CC $CFLAGS -o $BUILD_DIR/arena_libc_backend_test.exe arena_libc_backend_test.c
$CC $CFLAGS -o $BUILD_DIR/darray_test.exe darray_test.c
