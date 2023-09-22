#define DARRAY_IMPLEMENTATION
#include "../darray.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

typedef struct {
    char name[255];
    int age;
} Entt;

Entt set_name(Entt ntt, const char* name)
{
    size_t name_len = strlen(name);
    for(size_t i = 0; i < sizeof(ntt.name); ++i)
        ntt.name[i] = i < name_len ? name[i] : 0;
    return ntt;
}

int main(void)
{
    darray_t entts = darray_of(Entt);
    Entt entt;
    entt.age = 19;
    entt = set_name(entt, "Foo");
    entts = darray_push(entts, &entt);
    entt = set_name(entt, "Bar");
    entt.age = 20;
    entts = darray_push(entts, &entt);
    entt = set_name(entt, "Buzz");
    entt.age = 20;
    entts = darray_push(entts, &entt);

    assert(entts.count == 3);

    darray_get(entts, 0, &entt);
    assert(strcmp(entt.name, "Foo") == 0);
    entt = set_name(entt, "Hello");
    darray_set(entts, 1, &entt);
    darray_get(entts, 1, &entt);
    assert(strcmp(entt.name, "Hello") == 0);

    entts = darray_pop(entts, 1);
    assert(entts.count == 2);
    printf("TEST IS DONE\n");
}
