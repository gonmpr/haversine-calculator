#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    double x0;
    double y0;
    double x1;
    double y1;
} pair_t;


bool load_pairs_file(const char *filename,
                     pair_t **out_pairs,
                     size_t *out_count);
