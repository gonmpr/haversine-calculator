#include <stdio.h>
#include <stdlib.h>

#include "pairs_parser.h"
#include "parser/jparse.h"

bool load_pairs_file(const char *filename,
                     pair_t **out_pairs,
                     size_t *out_count) {

  if (!filename || !out_pairs || !out_count)
      return false;

  *out_pairs = NULL;
  *out_count = 0;

  json_value_t root;
  if (!json_parse_file(filename, &root)) {
      printf("error parsing json\n");
      return false;
  }

  if (root.type != JSON_OBJECT) {
      printf("root is not an object\n");
      json_value_free(&root);
      return false;
  }

  json_value_t *pairs_value = json_object_get(root.object, "pairs");
  if (!pairs_value || pairs_value->type != JSON_ARRAY) {
      printf("missing pairs array\n");
      json_value_free(&root);
      return false;
  }

  json_array_t *json_pairs = pairs_value->array;

  pair_t *pairs = malloc(sizeof(pair_t) * json_pairs->count);
  if (!pairs) {
      json_value_free(&root);
      return false;
  }

  for (size_t i = 0; i < json_pairs->count; i++) {

      json_value_t *item = json_array_get(json_pairs, i);

      if (!item || item->type != JSON_OBJECT) {
          free(pairs);
          json_value_free(&root);
          return false;
      }

      json_value_t *x0 = json_object_get(item->object, "x0");
      json_value_t *y0 = json_object_get(item->object, "y0");
      json_value_t *x1 = json_object_get(item->object, "x1");
      json_value_t *y1 = json_object_get(item->object, "y1");

      if (!x0 || !y0 || !x1 || !y1) {
          free(pairs);
          json_value_free(&root);
          return false;
      }

      if (x0->type != JSON_NUMBER ||
          y0->type != JSON_NUMBER ||
          x1->type != JSON_NUMBER ||
          y1->type != JSON_NUMBER) {
          free(pairs);
          json_value_free(&root);
          return false;
      }

      pairs[i].x0 = x0->number;
      pairs[i].y0 = y0->number;
      pairs[i].x1 = x1->number;
      pairs[i].y1 = y1->number;
  }

  *out_pairs = pairs;
  *out_count = json_pairs->count;

  json_value_free(&root);

  return true;
}
