#include "9ccs.h"

Vector *new_vec() {
    Vector *v = malloc(sizeof(Vector));
    v->data = malloc(sizeof(void *) * 16);
    v->capacity = 16;
    v->len = 0;
    return v;
}

void vec_push(Vector *v, void *elem) {
    if (v->len == v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, sizeof(void *) * v->capacity);
    }
    v->data[v->len++] = elem;
}

void vec_pushi(Vector *v, int val) {
    vec_push(v, (void *)(intptr_t)val);
}

void *vec_pop(Vector *v) {
    assert(v->len);
    return v->data[--v->len];
}

void *vec_last(Vector *v) {
    assert(v->len);
    return v->data[v->len - 1];
}

bool vec_contains(Vector *v, void *elem) {
    for (int i = 0; i < v->len; i++) {
        if (v->data[i] == elem) {
            return true;
        }
    }
    return false;
}

bool vec_union1(Vector *v, void *elem) {
    if (vec_contains(v, elem)) {
        return false;
    }
    vec_push(v, elem);
    return true;
}

Map *new_map(void) {
    Map *map = malloc(sizeof(Map));
    map->keys = new_vec();
    map->vals = new_vec();
    return map;
}

void map_put(Map *map, char *key, void *val) {
    vec_push(map->keys, key);
    vec_push(map->vals, val);
}

void map_puti(Map *map, char *key, int val) {
    map_put(map, key, (void *)(intptr_t)val);
}

void *map_get(Map *map, char *key) {
    for (int i = map->keys->len - 1; i >= 0; i--) {
        if (!strcmp(map->keys->data[i], key)) {
            return map->vals->data[i];
        }
    }
    return NULL;
}

int map_geti(Map *map, char *key, int default_) {
    for (int i = map->keys->len - 1; i >= 0; i--) {
        if (!strcmp(map->keys->data[i], key)) {
            return (intptr_t)map->vals->data[i];
        }
    }
    return default_;
}

int size_of(Type *ty) {
    if (ty->ty == INT) {
        return 4;
    }
    assert(ty->ty == PTR);
    return 8;
}

int calc_need_byte(Type *ty) {
    if (ty->ty == ARRAY) {
        return size_of(ty->ptr_to) * ty->array_size;
    } 
    return 8;
}