#include <stdio.h>
#include <stdlib.h>

#include "pairs_parser.h"
#include "reference_haversine.h"
#include "timer/timer.h"

typedef unsigned long long u64;

int main(void){
  
  pair_t *pairs = NULL;
  size_t count = 0;

  u64 time_start = read_cpu_clock();

  if (!load_pairs_file("input/input.json", &pairs, &count)) {
      printf("could not load pairs\n");
      return 1;
  }

  for (size_t i = 0; i < count; i++) {
      printf(
          "x0:%f y0:%f -> x1:%f y1:%f | ",
          pairs[i].x0,
          pairs[i].y0,
          pairs[i].x1,
          pairs[i].y1
      );
      printf("d: %f\n", reference_haversine(pairs[i]));
  }

  u64 time_end = read_cpu_clock();

  printf("CPU FREQUENCY: %llu Hz\n", get_cpu_freq());
  printf("CYCLES: %llu\n", time_end - time_start);

  free(pairs);
}

