#define COMMON_IMPLEMENTATION
#include "../common.h"

#include <stdio.h>

int main(void)
{
    String_View text = sv_from_cstr("Hello, World");

    String_View comparator = sv_from_cstr("Hell");
    printf("\""SV_FMT"\" has prefix \"" SV_FMT "\" => %s\n", 
            SV_ARGV(text),
            SV_ARGV(comparator),
            sv_has_prefix(text, comparator) ? "true" : "false"
            );

    comparator = sv_from_cstr("hell");
    printf("\""SV_FMT"\" has prefix \"" SV_FMT "\" => %s\n", 
            SV_ARGV(text),
            SV_ARGV(comparator),
            sv_has_prefix(text, comparator) ? "true" : "false"
            );

    comparator = sv_from_cstr("orld");
    printf("\""SV_FMT"\" has suffix \"" SV_FMT "\" => %s\n", 
            SV_ARGV(text),
            SV_ARGV(comparator),
            sv_has_suffix(text, comparator) ? "true" : "false"
            );

    comparator = sv_from_cstr("orlD");
    printf("\""SV_FMT"\" has suffix \"" SV_FMT "\" => %s\n", 
            SV_ARGV(text),
            SV_ARGV(comparator),
            sv_has_suffix(text, comparator) ? "true" : "false"
            );

    comparator = sv_from_cstr("Hello, World");
    printf("\""SV_FMT"\" is equal to \"" SV_FMT "\" => %s\n", 
            SV_ARGV(text),
            SV_ARGV(comparator),
            sv_eq(text, comparator) ? "true" : "false"
            );

    comparator = sv_from_cstr("hello, world");
    printf("\""SV_FMT"\" is equal to \"" SV_FMT "\" => %s\n", 
            SV_ARGV(text),
            SV_ARGV(comparator),
            sv_eq(text, comparator) ? "true" : "false"
            );

    comparator = sv_from_cstr("llo, Wor");
    printf("\""SV_FMT"\" contains \"" SV_FMT "\" => %s\n", 
            SV_ARGV(text),
            SV_ARGV(comparator),
            sv_contains(text, comparator) ? "true" : "false"
            );

    comparator = sv_from_cstr("llo, wor");
    printf("\""SV_FMT"\" contains \"" SV_FMT "\" => %s\n", 
            SV_ARGV(text),
            SV_ARGV(comparator),
            sv_contains(text, comparator) ? "true" : "false"
            );
}
