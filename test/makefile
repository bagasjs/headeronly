CC := clang
CFLAGS := -Wall -Wextra -Wpedantic -I..

WASM_CFLAGS := --target=wasm32 -ffreestanding -nostdinc --no-standard-libraries
WASM_CFLAGS += -mbulk-memory -mreference-types -mmultivalue -mmutable-globals -mnontrapping-fptoint -msign-ext
WASM_LDFLAGS := -Wl,--export-all -Wl,--no-entry -Wl,--allow-undefined

BUILD_DIR := build
BINARIES += $(BUILD_DIR)/string_view_test
BINARIES += $(BUILD_DIR)/arena_libc_backend_test
# BINARIES := $(BUILD_DIR)/arena_wasm_backend_test

all: $(BUILD_DIR) $(BINARIES)

$(BUILD_DIR)/string_view_test: string_view_test.c
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/arena_libc_backend_test: arena_libc_backend_test.c
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/arena_wasm_backend_test: arena_wasm_backend_test.c
	$(CC) $(CFLAGS) $(WASM_CFLAGS) -o $@ $^ $(WASM_LDFLAGS)


$(BUILD_DIR):
	mkdir $@
