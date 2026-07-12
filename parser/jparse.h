#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct json_value_t json_value_t;
typedef struct json_array_t json_array_t;
typedef struct json_object_t json_object_t;
typedef struct json_pair_t json_pair_t;

typedef enum {
    JSON_PARSING_ERROR,

    JSON_NULL,
    JSON_BOOL,
    JSON_NUMBER,
    JSON_ARRAY,
    JSON_STRING,
    JSON_OBJECT,
} json_data_type;

struct json_value_t {
    json_data_type type;

    union {
        double        number;
        int           boolean;
        char          *string;
        json_array_t  *array;
        json_object_t *object;
    };
};

struct json_array_t {
    json_value_t *items;
    size_t       count;
    size_t       capacity;
};

struct json_pair_t {
    char         *key;
    json_value_t value;
};

struct json_object_t {
    json_pair_t  *pairs;
    size_t       count;
    size_t       capacity;
};


bool json_parse_file(const char *filename, json_value_t *out);
void json_value_free(json_value_t *value);

json_value_t *json_object_get(json_object_t *object, const char *key);
json_value_t *json_array_get(json_array_t *array, size_t index);
size_t json_array_count(json_array_t *array);
