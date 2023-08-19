#define STRING_VIEW_IMPLEMENTATION
#include "../string_view.h"

#include <stdio.h>

int main(void)
{
    String_View sv = sv_from_cstr("Hello, World");
    printf("%s\n", sv_has_prefix(sv, sv_from_cstr("Hell")) ? "true" : "false");
    printf("%s\n", sv_has_prefix(sv, sv_from_cstr("hell")) ? "true" : "false");
    printf("%s\n", sv_has_suffix(sv, sv_from_cstr("orld")) ? "true" : "false");
    printf("%s\n", sv_has_suffix(sv, sv_from_cstr("orlD")) ? "true" : "false");
    printf("%s\n", sv_eq(sv, sv_from_cstr("Hello, World")) ? "true" : "false");
    printf("%s\n", sv_eq(sv, sv_from_cstr("World")) ? "true" : "false");
    printf("%s\n", sv_contains(sv, sv_from_cstr("llo, Worl")) ? "true" : "false");
}
