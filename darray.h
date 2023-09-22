#ifndef DARRAY_H
#define DARRAY_H

#include <stddef.h> // size_t, sizeof
#include <string.h> // memset, memcpy

#ifndef DARRAY_ASSERT
    #include <assert.h>
    #ifndef NDEBUG
        #define DARRAY_ASSERT(COND) assert((COND))
    #else
        #define DARRAY_ASSERT(COND)
    #endif
#endif

#if !defined(DARRAY_MALLOC) && !defined(DARRAY_FREE)
    #include <stdlib.h>
    #define DARRAY_MALLOC malloc
    #define DARRAY_FREE free
#endif

#if !defined(DARRAY_MALLOC) || !defined(DARRAY_FREE)
    #error "DARRAY_MALLOC and DARRAY_FREE should be defined"
#endif

typedef struct {
    size_t count, stride, capacity;
    void* data;
} darray_t;

darray_t darray_new(size_t capacity, size_t stride);
void darray_free(darray_t arr);
darray_t darray_reallocate(darray_t arr, size_t new_capacity);
int darray_get(darray_t arr, size_t index, void* writable);
int darray_set(darray_t arr, size_t index, const void* readonly);
darray_t darray_push(darray_t arr, const void* item);
darray_t darray_pop(darray_t arr, size_t count);

#define darray_of(TYPE) darray_new(2, sizeof(TYPE))
#define DARRAY_NULL (darray_t){0}

#endif // DARRAY_H

#ifdef DARRAY_IMPLEMENTATION

darray_t darray_new(size_t capacity, size_t stride)
{
    darray_t arr;
    arr.capacity = capacity;
    arr.stride = stride;
    arr.count = 0;
    size_t bytes_capacity = arr.stride * arr.capacity;
    arr.data = DARRAY_MALLOC(bytes_capacity);
    memset(arr.data, 0, arr.capacity*arr.stride);
    return arr;
}

void darray_free(darray_t arr)
{
    DARRAY_ASSERT(arr.data != NULL);
    DARRAY_FREE(arr.data);
}

darray_t darray_reallocate(darray_t arr, size_t new_capacity)
{

    if(arr.capacity == 0) // just return the string if it's a view-only
        return arr;
    if(new_capacity <= arr.capacity)
        return arr;
    void* old_data = arr.data;
    arr.data = DARRAY_MALLOC(arr.stride * new_capacity);
    DARRAY_ASSERT(arr.data != NULL);
    for(size_t i = 0; i < new_capacity * arr.stride; ++i)
        ((char*)arr.data)[i] = i < (arr.count * arr.stride) ? ((char*)old_data)[i] : 0;
    arr.capacity = new_capacity;

    DARRAY_FREE(old_data);
    return arr;
}

darray_t darray_push(darray_t arr, const void* item)
{
    if(arr.count + 1 > arr.capacity) {
        arr = darray_reallocate(arr, arr.capacity * 3/2);
    }
    size_t offset = arr.count * arr.stride;
    memcpy((void*)((size_t)arr.data + offset), item, arr.stride);
    arr.count += 1;
    return arr;
}

darray_t darray_pop(darray_t arr, size_t count)
{
    if(count > arr.count)
        return arr;
    arr.count -= count;
    return arr;
}

int darray_get(darray_t arr, size_t index, void* dst)
{
    if(index >= arr.count)
        return 0;
    size_t offset = index * arr.stride;
    memcpy(dst, (void*)((size_t)arr.data + offset), arr.stride);
    return 1;
}

int darray_set(darray_t arr, size_t index, const void* item)
{
    if(index >= arr.count)
        return 0;
    size_t offset = index * arr.stride;
    memcpy((void*)((size_t)arr.data + offset), item, arr.stride);
    return 1;
}

#endif // DARRAY_IMPLEMENTATION
