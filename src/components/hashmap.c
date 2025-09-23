#ifndef __CLIC_COMPONENTS_HASHMAP_C__
#define __CLIC_COMPONENTS_HASHMAP_C__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "./error.c"

typedef struct Clic_HashmapEntry {
    char* key;
    void* value;
    struct Clic_HashmapEntry* next;
} Clic_HashmapEntry;

typedef struct Clic_Hashmap {
    size_t capacity;
    size_t length;
    Clic_HashmapEntry** buckets;
} Clic_Hashmap;

static uint64_t _clic_hashmap_hash_string(const char* str) {
    uint64_t hash = 1469598103934665603ULL;
    while (*str) {
        hash ^= (uint8_t)(*str++);
        hash *= 1099511628211ULL;
    }
    return hash;
}

static Clic_HashmapEntry* _clic_hashmap_entry_new(const char* key, void* value) {
    Clic_HashmapEntry* entry = (Clic_HashmapEntry*)malloc(sizeof(Clic_HashmapEntry));
    if (!entry)
        return NULL;
    size_t key_len = strlen(key);
    entry->key = (char*)malloc(key_len + 1);
    if (!entry->key) {
        free(entry);
        return NULL;
    }
    memcpy(entry->key, key, key_len + 1);
    entry->value = value;
    entry->next = NULL;
    return entry;
}

static Clic_Error _clic_hashmap_grow(Clic_Hashmap* map) {
    size_t new_capacity = map->capacity * 2;
    Clic_HashmapEntry** new_buckets = (Clic_HashmapEntry**)calloc(new_capacity, sizeof(Clic_HashmapEntry*));
    if (!new_buckets)
        return (Clic_Error){.code = -1, .message = "Failed to allocate hashmap buckets"};

    for (size_t i = 0; i < map->capacity; i++) {
        Clic_HashmapEntry* entry = map->buckets[i];
        while (entry) {
            Clic_HashmapEntry* next = entry->next;
            uint64_t hash = _clic_hashmap_hash_string(entry->key);
            size_t index = (size_t)(hash % new_capacity);
            entry->next = new_buckets[index];
            new_buckets[index] = entry;
            entry = next;
        }
    }

    free(map->buckets);
    map->buckets = new_buckets;
    map->capacity = new_capacity;
    return (Clic_Error){0};
}

Clic_Hashmap clic_hashmap_init() {
    const size_t initial_capacity = 16;
    Clic_Hashmap map;
    map.capacity = initial_capacity;
    map.length = 0;
    map.buckets = (Clic_HashmapEntry**)calloc(initial_capacity, sizeof(Clic_HashmapEntry*));
    return map;
}

Clic_Error clic_hashmap_set(Clic_Hashmap* map, char* key, void* value) {
    if (!map || !key || !map->buckets)
        return (Clic_Error){.code = -1, .message = "Invalid hashmap or key"};

    if (map->capacity == 0) {
        *map = clic_hashmap_init();
        if (!map->buckets)
            return (Clic_Error){.code = -1, .message = "Failed to initialize hashmap"};
    }

    if ((map->length + 1) * 1.0 / map->capacity > 0.75) {
        Clic_Error grow_err = _clic_hashmap_grow(map);
        if (grow_err.code != 0)
            return grow_err;
    }

    uint64_t hash = _clic_hashmap_hash_string(key);
    size_t index = (size_t)(hash % map->capacity);
    Clic_HashmapEntry* head = map->buckets[index];

    for (Clic_HashmapEntry* it = head; it; it = it->next) {
        if (strcmp(it->key, key) == 0) {
            it->value = value;
            return (Clic_Error){0};
        }
    }

    Clic_HashmapEntry* new_entry = _clic_hashmap_entry_new(key, value);
    if (!new_entry)
        return (Clic_Error){.code = -1, .message = "Failed to allocate hashmap entry"};

    new_entry->next = head;
    map->buckets[index] = new_entry;
    map->length++;
    return (Clic_Error){0};
}

void* clic_hashmap_get(Clic_Hashmap* map, char* key) {
    if (!map || !key || !map->buckets || map->capacity == 0) {
        return NULL;
    }

    uint64_t hash = _clic_hashmap_hash_string(key);
    size_t index = (size_t)(hash % map->capacity);

    for (Clic_HashmapEntry* it = map->buckets[index]; it; it = it->next) {
        if (strcmp(it->key, key) == 0) {
            return it->value;
        }
    }

    return NULL;
}

#endif /** __CLIC_COMPONENTS_HASHMAP_C__  */