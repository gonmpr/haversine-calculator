#include <stdio.h>
#include <stdlib.h>
#include "pairs_parser.h"

int main(void){
  pair_t *pairs = NULL;
  size_t count = 0;

  if (!load_pairs_file("input/input.json", &pairs, &count)) {
      printf("could not load pairs\n");
      return 1;
  }

  for (size_t i = 0; i < count; i++) {
      printf(
          "x0:%f y0:%f -> x1:%f y1:%f\n",
          pairs[i].x0,
          pairs[i].y0,
          pairs[i].x1,
          pairs[i].y1
      );
  }

  free(pairs);
}

