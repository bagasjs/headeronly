#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#if defined(__EMSCRIPTEN__) || defined(__wasm__) || defined(__wasm32__) || defined(__wasm64__)
    typedef typeof(sizeof(1)) size_t;
    #define STRING_VIEW_TARGET_WASM 1
    typedef enum { false = 0, true } bool;
#else
    #define STRING_VIEW_TARGET_WASM 0
    #include <stddef.h>
    #include <stdbool.h>
#endif

typedef struct String_View {
    const char* data;
    size_t size;
} String_View;

#define SV_FMT "%.*s"
#define SV_ARGV(sv) (int)sv.size, sv.data
#define SV_NULL (String_View){0}

#define CAPACITY_MULTIPLIER 2

String_View sv_from_cstr(const char* data);
String_View sv_slice(String_View strv, size_t start, size_t end);

int sv_find_cstr(String_View strv, const char* sth, size_t index);
int sv_find(String_View strv, String_View sth, size_t index);

bool sv_contains(String_View strv, String_View sth);
bool sv_has_prefix(String_View strv, String_View prefix);
bool sv_has_suffix(String_View strv, String_View suffix);
bool sv_eq(String_View a, String_View b);
void sv_split(String_View strv, String_View sep, String_View* results, size_t* results_size);

#endif // STRING_VIEW_H

#ifdef STRING_VIEW_IMPLEMENTATION

size_t _strlen(const char* cstr) 
{
    size_t i = 0;
    while(cstr[++i] != '\0');
    return i;
}

String_View sv_from_cstr(const char* data)
{
    return (String_View) {
        .data = data,
        .size = _strlen(data),
    };
}

String_View sv_slice(String_View strv, size_t start, size_t end)
{
    if(end < start) {
        size_t swapper = end;
        end = start;
        start = swapper;
    }

    if(strv.size < start) 
        return SV_NULL;

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

int sv_find_cstr(String_View strv, const char* sth, size_t index)
{
    return sv_find(strv, sv_from_cstr(sth), index);
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

#endif // STRING_VIEW_IMPLEMENTATION
