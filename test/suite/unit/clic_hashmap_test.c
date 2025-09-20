#include <assert.h>
#include <stdio.h>

#include "../../../include/main.c"

int main(int argc, char *argv[]) {
    Clic_Hashmap hashmap = clic_hashmap_init();

    clic_hashmap_set(&hashmap, "key", "value");
    clic_hashmap_set(&hashmap, "key:2", "value:2");

    assert(strcmp(clic_hashmap_get(&hashmap, "key"), "value") == 0);
    assert(strcmp(clic_hashmap_get(&hashmap, "key:2"), "value:2") == 0);

    clic_hashmap_set(&hashmap, "key", "other-value");

    assert(strcmp(clic_hashmap_get(&hashmap, "key"), "other-value") == 0);

    printf("[SUCCESS]: clic_hashmap_test\n");

    return 0;
}