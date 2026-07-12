#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parser.h"
#include "load_file.h"

bool json_parse_file(const char *filename, json_value_t *out)
{
    if (!out) return false;

    json_file_t file = load_json_file(filename);
    if (!file.start) {
        out->type = JSON_PARSING_ERROR;
        return false;
    }

    *out = parse_value(&file);

    skip_whitespace(&file);

    bool ok = out->type != JSON_PARSING_ERROR && file.cursor == file.end;

    free(file.start);

    if (!ok) {
        json_value_free(out);
        out->type = JSON_PARSING_ERROR;
        return false;
    }

    return true;
}


// busca en json por key
json_value_t *json_object_get(json_object_t *object, const char *key)
{
    if (!object || !key) return NULL;

    for (size_t i = 0; i < object->count; i++) {
        if (strcmp(object->pairs[i].key, key) == 0) {
            return &object->pairs[i].value;
        }
    }

    return NULL;
}

//busca en array por indice
json_value_t *json_array_get(json_array_t *array, size_t index)
{
    if (!array) return NULL;
    if (index >= array->count) return NULL;

    return &array->items[index];
}

size_t json_array_count(json_array_t *array)
{
    if (!array) return 0;
    return array->count;
}
