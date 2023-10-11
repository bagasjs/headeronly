/**
 * `common.h` - Common structs and functions that I don't want to write over again and again
 *
 */
#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdbool.h>

void* __common_memcpy(void* dst, const void* src, size_t size);
size_t __common_strlen(const char* cstr);

bool __common_iswhitespace(char c);
bool __common_isalpha(char c);
bool __common_isalnum(char c);
bool __common_isdigit(char c);

#if !defined(COMMON_ASSERT)
    #include <assert.h>
    #define COMMON_ASSERT assert
#endif

#if !defined(COMMON_MALLOC) && !defined(COMMON_FREE) && !defined(COMMON_REALLOC)
    #include <stdlib.h>
    #define COMMON_MALLOC malloc
    #define COMMON_FREE free
    #define COMMON_REALLOC realloc
#endif

#if !defined(COMMON_ASSERT)
    #error "`common.h` require the definition of `COMMON_ASSERT()` macro"
#endif
#if !defined(COMMON_MALLOC) || !defined(COMMON_FREE) || !defined(COMMON_REALLOC)
    #error "`common.h` requires you to define `COMMON_ALLOC()`, `COMMON_FREE()`, `COMMON_REALLOC()` macros"
#endif

#define CAST(T, a) ((T)(a))
#define SWAP(T, a, b)   \
    do {                \
        T tmp = a;      \
        a = b;          \
        b = tmp;        \
    } while(0)

#define DA_INIT_CAPACITY 32
#define da(T) struct { T* data; size_t count, capacity; }
#define da_free(da) COMMON_FREE((da)->data)
#define da_append(da, item) \
    do {                                                            \
        if((da)->count >= (da)->capacity) {                         \
            size_t new_capacity = (da)->capacity * 2;               \
            if(new_capacity == 0) new_capacity = DA_INIT_CAPACITY;  \
            (da)->data = COMMON_REALLOC((da)->data,                 \
                    new_capacity * sizeof(*(da)->data));            \
            (da)->capacity = new_capacity;                          \
        }                                                           \
        (da)->data[(da)->count++] = (item);                         \
    } while(0)

#define da_append_many(da, new_items, new_items_count) \
    do {                                                                \
        if((da)->count + new_items_count > (da)->capacity) {            \
            if((da)->capacity == 0) (da)->capacity = DA_INIT_CAPACITY;  \
            (da)->capacity = (da)->capacity * 2 + new_items_count;      \
            (da)->data = COMMON_REALLOC((da)->data,                     \
                    (da)->capacity * sizeof(*(da)->data));              \
        }                                                               \
        __common_memcpy((da)->data + (da)->count, new_items,            \
                new_items_count * sizeof(*(da)->data));                 \
        (da)->count += new_items_count;                                 \
    } while(0)

typedef struct {
    const char* data;
    size_t size;
} String_View;

#define SV_FMT "%.*s"
#define SV_ARGV(sv) (int)sv.size, sv.data
#define SV_DEBUG_FMT "(%zu) %.*s"
#define SV_DEBUG_ARGV(sv) sv.size, (int)sv.size, sv.data
#define INVALID_SV (String_View){0}

String_View sv_from_cstr(const char* data);
String_View sv_slice(String_View strv, size_t start, size_t end);

int sv_find_cstr(String_View strv, const char* sth, size_t index);
int sv_find(String_View strv, String_View sth, size_t index);

bool sv_contains(String_View strv, String_View sth);
bool sv_has_prefix(String_View strv, String_View prefix);
bool sv_has_suffix(String_View strv, String_View suffix);
bool sv_eq(String_View a, String_View b);

String_View sv_ltrim(String_View strv);
String_View sv_rtrim(String_View strv);

typedef da(char) String_Builder;
#define sb_append(sb, cstr, cstr_length) da_append_many(sb, cstr, cstr_length + 1)
#define sb_append_cstr(sb, cstr) da_append_many(sb, cstr, __common_strlen(cstr) + 1)
#define sb_free(sb) da_free(sb)

#endif // COMMON_H

#ifdef COMMON_IMPLEMENTATION

void* __common_memcpy(void* dst, const void* src, size_t size)
{
    for(size_t i = 0; i < size; ++i)
        CAST(char*, dst)[i] = CAST(char*, src)[i];
    return dst;
}

size_t __common_strlen(const char* cstr)
{
    size_t i = 0;
    while(cstr[++i] != '\0');
    return i;
}

bool __common_iswhitespace(char ch)
{
    return ch == '\n' || ch == '\t' || ch == ' ' || ch == '\r';
}

bool __common_isalpha(char c)
{
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

bool __common_isdigit(char c)
{
    return ('0' <= c && c <= '9');
}

bool __common_isalnum(char c)
{
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9');
}

String_View sv_from_cstr(const char* data)
{
    return (String_View) {
        .data = data,
        .size = __common_strlen(data),
    };
}

String_View sv_slice(String_View strv, size_t start, size_t end)
{
    if(end < start) SWAP(size_t, start, end);

    if(strv.size < start) 
        return INVALID_SV;

    return (String_View) {
        .data = strv.data + start,
        .size = end - start,
    };
}

bool sv_eq(String_View a, String_View b)
{
    if(a.size < b.size)
        return false;
    for(size_t i = 0; i < b.size; ++i) {
        if(a.data[i] != b.data[i]) 
            return false;
    }
    return true;
}

bool sv_contains(String_View strv, String_View sth)
{
    if(strv.size < sth.size)
        return false;
    for(size_t i = 0; i < strv.size; ++i) {
        if(strv.data[i] == sth.data[0]) {
            String_View cmp = sv_slice(strv, i, i + sth.size);
            if(sv_eq(sth, cmp))
                return true;
        }
    }
    return false;
}

bool sv_has_prefix(String_View strv, String_View prefix)
{
    if(strv.size < prefix.size) {
        return false;
    }

    for(size_t i = 0; i < prefix.size; ++i) {
        if(strv.data[i] != prefix.data[i])
            return false;
    }
    return true;
}

bool sv_has_suffix(String_View strv, String_View suffix)
{
    if(strv.size < suffix.size)
        return false;
    for(int i = (int)suffix.size - 1; i >= 0; --i) {
        if(strv.data[strv.size - suffix.size + i] != suffix.data[i])
            return false;
    }
    return true;
}

int sv_find(String_View strv, String_View sth, size_t index)
{
    if(strv.size < sth.size)
        return -1;

    size_t found_count = 0;
    for(size_t i = 0; i < strv.size; ++i) {
        if(strv.data[i] == sth.data[0]) {
            String_View cmp = sv_slice(strv, i, i + sth.size);
            if(sv_eq(sth, cmp)) {
                if(found_count == index)
                    return (int)i;
                ++found_count;
            }
        }
    }

    return -1;
}

String_View sv_ltrim(String_View strv)
{
    size_t i = 0;
    while(__common_iswhitespace(strv.data[i]))
        i += 1;
    strv.data += i;
    strv.size -= i;
    return strv;
}

String_View sv_rtrim(String_View strv)
{
    if(strv.size == 0) return INVALID_SV;
    size_t i = 0;
    while(__common_iswhitespace(strv.data[strv.size - i - 1]))
        i += 1;
    strv.size -= i;
    return strv;
}

#endif // COMMON_IMPLEMENTATION
