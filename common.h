/**
 * `common.h` - Common structs and functions that I don't want to write over again and again
 *
 */
#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(__WIN32__)
    #include <windows.h>
    #include <windowsx.h>
    #define PLATFORM_WINDOWS 1
    #define PLATFORM_DESKTOP 1
    #ifndef _WIN64
        #error "Only supporting 64-bit windows"
    #endif
#elif defined(__ANDROID__)
    #define PLATFORM_ANDROID 1
    #define PLATFORM_LINUX 1
    #define PLATFORM_MOBILE 1
#elif defined(__gnu_linux__) || defined(__linux__)
    #define PLATFORM_LINUX 1
    #define PLATFORM_DESKTOP 1
#elif __APPLE__
    #define PLATFORM_APPLE 1
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
        #define PLATFORM_MOBILE 1
    #elif TARGET_OS_MAC
        #define PLATFORM_DESKTOP 1
    #else
        #warn "Unknown apple platform defaulting to desktop environment"
        #define PLATFORM_DESKTOP 1
    #endif
    #error "Apple platform is unsupported currently"
#elif defined(__unix__)
    #define PLATFORM_UNIX 1
    #error "Unix platform is unsupported"
#elif defined(_POSIX_VERSION)
    #define PLATFORM_POSIX 1
    #error "Unknown posix platform, only supporting linux, android and apple platforms"
#else
    #error "Unsupported platform"
#endif

#ifndef PLATFORM_DESKTOP
    #define PLATFORM_DESKTOP 0
#endif
#ifndef PLATFORM_MOBILE
    #define PLATFORM_MOBILE 0
#endif
#ifndef PLATFORM_WINDOWS
    #define PLATFORM_WINDOWS 0
#endif
#ifndef PLATFORM_ANDROID
    #define PLATFORM_ANDROID 0
#endif
#ifndef PLATFORM_APPLE
    #define PLATFORM_APPLE 0
#endif
#ifndef PLATFORM_LINUX
    #define PLATFORM_LINUX 0
#endif
#ifndef PLATFORM_UNIX
    #define PLATFORM_UNIX 0
#endif
#ifndef PLATFORM_POSIX
    #define PLATFORM_POSIX 0
#endif

#if defined(__clang__)
    #define CC_CLANG 1
#elif defined(_MSC_VER)
    #if !PLATFORM_WINDOWS
        #error "Unreachable: MSVC should only be in Windows"
    #endif
    #define CC_MSVC 1
#elif defined(__GNUC__) || defined(__GNUG__)
    #define CC_GCC 1
#else
    #error "Unsupported compilers"
#endif

#ifndef CC_CLANG
    #define CC_CLANG 0
#endif
#ifndef CC_MSVC
    #define CC_MSVC 0
#endif
#ifndef CC_GCC
    #define CC_GCC 0
#endif

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
    size_t count;
} String_View;

#define SV_FMT "%.*s"
#define SV_ARGV(sv) (int)sv.count, sv.data
#define SV_DEBUG_FMT "(%zu) \"%.*s\""
#define SV_DEBUG_ARGV(sv) sv.count, (int)sv.count, sv.data
#define INVALID_SV (String_View){0}

String_View sv_from_parts(const char* data, size_t n);
String_View sv_from_cstr(const char* cstr);
String_View sv_slice(String_View strv, size_t start, size_t end);

int sv_find_cstr(String_View strv, const char* sth, size_t index);
int sv_find(String_View strv, String_View sth, size_t index);

bool sv_contains(String_View strv, String_View sth);
bool sv_has_prefix(String_View strv, String_View prefix);
bool sv_has_suffix(String_View strv, String_View suffix);
bool sv_eq(String_View a, String_View b);

String_View sv_ltrim(String_View strv);
String_View sv_rtrim(String_View strv);
String_View sv_take_left_while(String_View strv, bool (*predicate)(char x));
String_View sv_chop_left(String_View* strv, size_t n);
String_View sv_chop_right(String_View* strv, size_t n);
String_View sv_chop_left_while(String_View* strv, bool (*predicate)(char x));
String_View sv_chop_by_delim(String_View* strv, char delim);
String_View sv_chop_by_sv(String_View* strv, String_View sv);
int sv_to_int(String_View strv);

typedef da(char) String_Builder;
#define sb_append(sb, cstr, cstr_length) da_append_many(sb, cstr, cstr_length + 1)
#define sb_append_cstr(sb, cstr) da_append_many(sb, cstr, __common_strlen(cstr) + 1)
#define sb_free(sb) da_free(sb)

#ifndef COMMON_PLATFORM_INDEPENDENT

typedef da(String_View) Path_List;
bool mkdir_if_not_exists(const char* path);
bool copy_file(const char* dst_path, const char* src_path);
bool copy_dir_recursive(const char* dst_path, const char* src_path);
bool read_dir(const char* path, Path_List* children);

bool load_file_data(const char* path, String_Builder* sb);
bool save_file_data(const char* path, const void* data, size_t size);

#endif // COMMON_PLATFORM_INDEPENDENT

typedef enum {
    TRACE_LOG_INFO = 0,
    TRACE_LOG_WARN = 1,
    TRACE_LOG_ERROR = 2,
    TRACE_LOG_FATAL = 3,
} Trace_Log_Level;

void trace_log(Trace_Log_Level level, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // COMMON_H

#ifdef COMMON_IMPLEMENTATION

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifndef COMMON_PLATFORM_INDEPENDENT
    #if PLATFORM_WINDOWS
        #define WIN32_LEAN_AND_MEAN
        #include <windows.h>
        #include <direct.h>
        #include <shellapi.h>
        struct dirent {
            char d_name[MAX_PATH+1];
        };
        typedef struct DIR DIR;
        DIR* opendir(const char* dirpath);
        struct dirent* readdir(DIR* dirp);
        int closedir(DIR* dirp);
    #else
        #include <dirent.h>
        #include <sys/types.h>
        #include <sys/wait.h>
        #include <sys/stat.h>
        #include <unistd.h>
        #include <fcntl.h>
    #endif
#endif

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

String_View sv_from_parts(const char* data, size_t n)
{
    return (String_View) {
        .data = data,
        .count = n,
    };
}
String_View sv_from_cstr(const char* data)
{
    return (String_View) {
        .data = data,
        .count = __common_strlen(data),
    };
}

String_View sv_slice(String_View strv, size_t start, size_t end)
{
    if(end < start) SWAP(size_t, start, end);

    if(strv.count < start) 
        return INVALID_SV;

    return (String_View) {
        .data = strv.data + start,
        .count = end - start,
    };
}

bool sv_eq(String_View a, String_View b)
{
    if(a.count < b.count)
        return false;
    for(size_t i = 0; i < b.count; ++i) {
        if(a.data[i] != b.data[i]) 
            return false;
    }
    return true;
}

bool sv_contains(String_View strv, String_View sth)
{
    if(strv.count < sth.count)
        return false;
    for(size_t i = 0; i < strv.count; ++i) {
        if(strv.data[i] == sth.data[0]) {
            String_View cmp = sv_slice(strv, i, i + sth.count);
            if(sv_eq(sth, cmp))
                return true;
        }
    }
    return false;
}

bool sv_has_prefix(String_View strv, String_View prefix)
{
    if(strv.count < prefix.count) {
        return false;
    }

    for(size_t i = 0; i < prefix.count; ++i) {
        if(strv.data[i] != prefix.data[i])
            return false;
    }
    return true;
}

bool sv_has_suffix(String_View strv, String_View suffix)
{
    if(strv.count < suffix.count)
        return false;
    for(int i = (int)suffix.count - 1; i >= 0; --i) {
        if(strv.data[strv.count - suffix.count + i] != suffix.data[i])
            return false;
    }
    return true;
}

int sv_find(String_View strv, String_View sth, size_t index)
{
    if(strv.count < sth.count)
        return -1;

    size_t found_count = 0;
    for(size_t i = 0; i < strv.count; ++i) {
        if(strv.data[i] == sth.data[0]) {
            String_View cmp = sv_slice(strv, i, i + sth.count);
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
    strv.count -= i;
    return strv;
}

String_View sv_rtrim(String_View strv)
{
    if(strv.count == 0) return INVALID_SV;
    size_t i = 0;
    while(__common_iswhitespace(strv.data[strv.count - i - 1]))
        i += 1;
    strv.count -= i;
    return strv;
}

String_View sv_chop_right(String_View* strv, size_t n)
{
    if(n > strv->count) {
        n = strv->count;
    }

    String_View result = sv_from_parts(strv->data + strv->count - n, n);
    strv->count -= n;
    return result;
}

String_View sv_chop_left(String_View* strv, size_t n)
{
    if(n >= strv->count) {
        n = strv->count;
    }
    String_View result = sv_from_parts(strv->data, n);

    strv->count -= n;
    strv->data += n;
    return result;
}

String_View sv_chop_by_delim(String_View* strv, char delim)
{
    size_t i = 0;
    while(i < strv->count && strv->data[i] != delim) {
        i += 1;
    }

    String_View result = sv_from_parts(strv->data, i);
    if (i < strv->count) {
        strv->count -= i + 1;
        strv->data  += i + 1;
    } else {
        strv->count -= i;
        strv->data  += i;
    }
    return result;
}

String_View sv_chop_by_sv(String_View *sv, String_View thicc_delim)
{
    String_View window = sv_from_parts(sv->data, thicc_delim.count);
    size_t i = 0;
    while (i + thicc_delim.count < sv->count
        && !(sv_eq(window, thicc_delim)))
    {
        i++;
        window.data++;
    }

    String_View result = sv_from_parts(sv->data, i);

    if (i + thicc_delim.count == sv->count) {
        // include last <thicc_delim.count> characters if they aren't
        //  equal to thicc_delim
        result.count += thicc_delim.count;
    }

    // Chop!
    sv->data  += i + thicc_delim.count;
    sv->count -= i + thicc_delim.count;

    return result;
}

String_View sv_chop_left_while(String_View* strv, bool (*predicate)(char x))
{ 
    size_t i = 0;
    while (i < strv->count && predicate(strv->data[i])) {
        i += 1;
    }
    return sv_chop_left(strv, i);
}

String_View sv_take_left_while(String_View strv, bool (*predicate)(char x))
{
    size_t i = 0;
    while (i < strv.count && predicate(strv.data[i])) {
        i += 1;
    }
    return sv_from_parts(strv.data, i);
}

int sv_to_int(String_View strv)
{
    bool is_negative = false;
    if(strv.data[0] == '-') {
        is_negative = true;
        strv.count -= 1;
        strv.data += 1;
    }
    int result = 0;
    for (size_t i = 0; i < strv.count && __common_isdigit(strv.data[i]); ++i) {
        result = result * 10 + (int) strv.data[i] - '0';
    }
    if(is_negative) result *= -1;
    return result;
}

void trace_log(Trace_Log_Level level, const char* fmt, ...)
{
    FILE* f = level <= TRACE_LOG_WARN ? stdout : stderr;
    switch(level) {
        case TRACE_LOG_INFO: fprintf(f, "[INFO] ");
        case TRACE_LOG_WARN: fprintf(f, "[WARN] ");
        case TRACE_LOG_ERROR: fprintf(f, "[ERROR] ");
        case TRACE_LOG_FATAL: fprintf(f, "[FATAL] ");
    }

    va_list arg;
    va_start(arg, fmt);
    vfprintf(f, fmt, arg);
    va_end(arg);
    fprintf(f, "\n");
}

#ifndef COMMON_PLATFORM_INDEPENDENT

bool mkdir_if_not_exists(const char* path)
{
#if PLATFORM_WINDOWS
    int result = mkdir(path);
#else
    int result = mkdir(path, 0755);
#endif
    if(result < 0) {
        if(errno == EEXIST) {
            trace_log(TRACE_LOG_WARN, "Directory `%s` already exists", path);
            return true;
        }
        trace_log(TRACE_LOG_ERROR, "Could not create directory `%s`: `%s`", path, strerror(errno));
        return false;
    }
    return false;
}

bool copy_file(const char* dst_path, const char* src_path)
{
    COMMON_ASSERT(0 && "Not implemented");
    return false;
}

bool copy_dir_recursive(const char* dst_path, const char* src_path)
{
    COMMON_ASSERT(0 && "Not implemented");
    return false;
}

bool read_dir(const char* parent, Path_List* children)
{
    bool result = true;
    DIR *dir = NULL;

    dir = opendir(parent);
    if (dir == NULL) {
        trace_log(TRACE_LOG_ERROR, "Could not open directory %s: %s", parent, strerror(errno));
        if (dir) closedir(dir);
        return result;
    }

    errno = 0;
    struct dirent *ent = readdir(dir);
    while (ent != NULL) {
        da_append(children, nob_temp_strdup(ent->d_name));
        ent = readdir(dir);
    }

    if (errno != 0) {
        trace_log(TRACE_LOG_ERROR, "Could not read directory %s: %s", parent, strerror(errno));
        if (dir) closedir(dir);
        return result;
    }

    if (dir) closedir(dir);
    return result;
}

bool load_file_data(const char* path, String_Builder* sb)
{
}

bool save_file_data(const char* path, const void* data, size_t size)
{
}

#endif // COMMON_PLATFORM_INDEPENDENT

#if PLATFORM_WINDOWS

struct DIR
{
    HANDLE hFind;
    WIN32_FIND_DATA data;
    struct dirent* dirent;
};

DIR *opendir(const char *dirpath)
{
    COMMON_ASSERT(dirpath);

    char buffer[MAX_PATH];
    snprintf(buffer, MAX_PATH, "%s\\*", dirpath);

    DIR *dir = (DIR*)calloc(1, sizeof(DIR));

    dir->hFind = FindFirstFile(buffer, &dir->data);
    if (dir->hFind == INVALID_HANDLE_VALUE) {
        // TODO: opendir should set errno accordingly on FindFirstFile fail
        // https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
        errno = ENOSYS;
        goto fail;
    }

    return dir;

fail:
    if (dir) {
        free(dir);
    }

    return NULL;
}

struct dirent *readdir(DIR *dirp)
{
    COMMON_ASSERT(dirp);

    if (dirp->dirent == NULL) {
        dirp->dirent = (struct dirent*)calloc(1, sizeof(struct dirent));
    } else {
        if(!FindNextFile(dirp->hFind, &dirp->data)) {
            if (GetLastError() != ERROR_NO_MORE_FILES) {
                // TODO: readdir should set errno accordingly on FindNextFile fail
                // https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
                errno = ENOSYS;
            }

            return NULL;
        }
    }

    memset(dirp->dirent->d_name, 0, sizeof(dirp->dirent->d_name));

    strncpy(
        dirp->dirent->d_name,
        dirp->data.cFileName,
        sizeof(dirp->dirent->d_name) - 1);

    return dirp->dirent;
}

int closedir(DIR *dirp)
{
    COMMON_ASSERT(dirp);

    if(!FindClose(dirp->hFind)) {
        // TODO: closedir should set errno accordingly on FindClose fail
        // https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
        errno = ENOSYS;
        return -1;
    }

    if (dirp->dirent) {
        free(dirp->dirent);
    }
    free(dirp);

    return 0;
}

#endif // PLATFORM_WINDOWS

#endif // COMMON_IMPLEMENTATION

