CC := clang
CFLAGS := -Wall -Wextra -Wpedantic -I..

WASM_CFLAGS := --target=wasm32 -ffreestanding -nostdinc --no-standard-libraries
WASM_CFLAGS += -mbulk-memory -mreference-types -mmultivalue -mmutable-globals -mnontrapping-fptoint -msign-ext
WASM_LDFLAGS := -Wl,--export-all -Wl,--no-entry -Wl,--allow-undefined

all: arena_test_wasm_backend arena_test_libc_backend string_view_test

arena_test_wasm_backend: arena_test_wasm_backend.c
	$(CC) $(CFLAGS) $(WASM_CFLAGS) -o $@ $^ $(WASM_LDFLAGS)

arena_test_libc_backend: arena_test_libc_backend.c
	$(CC) $(CFLAGS) -o $@ $^

string_view_test: string_view_test.c
	$(CC) $(CFLAGS) -o $@ $^
