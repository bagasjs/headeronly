#ifndef ARENA_H
#define ARENA_H

#define ARENA_BACKEND_LIBC 0
#define ARENA_BACKEND_LINUX_MMAP 1
#define ARENA_BACKEND_WIN32_VIRTUALALLOC 2
#define ARENA_BACKEND_WASM_HEAPBASE 3

#if defined(__EMSCRIPTEN__) || defined(__wasm__) || defined(__wasm32__) || defined(__wasm64__)
    typedef typeof(sizeof(1)) size_t;
    #define ARENA_TARGET_WASM 1
#else
    #define ARENA_TARGET_WASM 0

    #include <stddef.h>
    #ifndef ARENA_ASSERT
        #include <assert.h>
        #define ARENA_ASSERT(X) assert((X))
    #endif
#endif


#ifndef ARENA_BACKEND
    #define ARENA_BACKEND ARENA_BACKEND_LIBC
#endif


typedef struct Region Region;
struct Region {
    Region* next;
    size_t usage, capacity;
    void* data;
};

#define REGION_DEFAULT_CAPACITY (8*1024)

Region* region_init(size_t capacity);
void region_deinit(Region* r);

typedef struct {
    Region* first;
    Region* last;
} Arena;

void* arena_alloc(Arena* a, size_t size);
void* arena_realloc(Arena* a, void* oldptr, size_t old_size, size_t new_size);
void arena_reset(Arena* a);
void arena_free(Arena* a);

char* arena_load_file_text(Arena* a, const char* file_path);
unsigned char* arena_load_file_data(Arena* a, const char* file_path);

#endif // ARENA_H

#ifdef ARENA_IMPLEMENTATION

void* arena_alloc(Arena* a, size_t size)
{
    if(a->last == NULL) {
        ARENA_ASSERT(a->first == NULL);
        size_t capacity = REGION_DEFAULT_CAPACITY;
        if(capacity < size) capacity = size;
        a->last = region_init(capacity);
        a->first = a->last;
    }

    while(a->last->usage + size > a->last->capacity && a->last->next != NULL)
    {
        a->last = a->last->next;
    }

    if(a->last->usage + size > a->last->capacity) {
        ARENA_ASSERT(a->last->next == NULL);
        size_t capacity = REGION_DEFAULT_CAPACITY;
        if(capacity < size) capacity = size;
        a->last = region_init(capacity);
        a->first = a->last;
    }

    void* result = (void*)((size_t)a->last->data + a->last->usage);
    a->last->usage += size;
    return result;
}

void arena_reset(Arena* a)
{
    for(Region* r = a->first; r != NULL; r = r->next) {
        r->usage = 0;
    }
    a->last = a->first;
}

void arena_free(Arena* a)
{
    Region* r = a->first;
    while(r) {
        Region* current = r;
        r = r->next;
        region_deinit(current);
    }
    a->first = NULL;
    a->last = NULL;
}


#if ARENA_BACKEND == ARENA_BACKEND_LIBC

#include <stdlib.h>

Region* region_init(size_t capacity)
{
    size_t size = sizeof(Region) + capacity;
    Region* r = (Region*)malloc(size);
    ARENA_ASSERT(r != NULL);
    r->next = NULL;
    r->usage = 0;
    r->capacity = capacity;
    r->data = (void*)(&r->data + sizeof(r->data));
    return r;
}

void region_deinit(Region* r)
{
    free(r);
}

#elif ARENA_BACKEND == ARENA_BACKEND_LINUX_MMAP
    #error "TODO: implement ARENA_BACKEND_LINUX_MMAP"
#elif ARENA_BACKEND == ARENA_BACKEND_WIN32_VIRTUALALLOC
    #ifndef _WIN32
        #error "ARENA_BACKEND_WIN32_VIRTUALALLOC is only available on Windows Platform"
    #endif
    #error "TODO: implement ARENA_BACKEND_WIN32_VIRTUALALLOC" 
#elif ARENA_BACKEND == ARENA_BACKEND_WASM_HEAPBASE
    #if !(ARENA_TARGET_WASM)
        #error "ARENA_BACKEND_WASM_HEAPBASE is only available for WebAssembly Platform"
    #endif
    #error "TODO: implement ARENA_BACKEND_WASM_HEAPBASE" 
#endif

#endif // ARENA_IMPLEMENTATION
