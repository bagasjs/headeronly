#define ARENA_IMPLEMENTATION
#include "../arena.h"
#include <assert.h>
#include <stdio.h>

typedef struct Entity {
    struct { int x, y; } pos;
    struct { int atk, def, spd; } stat;
} Entity;

int main(void)
{
    Arena arena = {0};
    Entity* entt1 = arena_alloc(&arena, sizeof(Entity));
    printf("arena.last->data = %p\n", arena.last->data);
    printf("arena.last->usage = %zu\n", arena.last->usage);
    printf("entt1 = %p\n", entt1);
    Entity* entt2 = arena_alloc(&arena, sizeof(Entity));
    printf("arena.last->usage = %zu\n", arena.last->usage);
    printf("entt2 = %p\n", entt2);
    assert(arena.last->usage == 2*sizeof(Entity));
    assert(entt1 == arena.last->data);
    assert(entt2 == arena.last->data + sizeof(Entity));
}
