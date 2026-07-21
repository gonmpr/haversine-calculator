#include <stdio.h>
#include <stdlib.h>

#include "pairs_parser.h"
#include "reference_haversine.h"
#include "profiler/profiler.h"

typedef unsigned long long u64;

int main(void){

  PROFILE_START(setting_up);

  pair_t *pairs = NULL;
  size_t count = 0;

  PROFILE_END(setting_up);

  PROFILE_BLOCK(json_parsing)
  {

  if (!load_pairs_file("input/input.json", &pairs, &count)) {
      printf("could not load pairs\n");
      return 1;
  }

  } //profile


  for (size_t i = 0; i < count; i++) {

    PROFILE_BLOCK(haversine)
    {
      reference_haversine(pairs[i]);
    } //profile

  }


  PROFILE_BLOCK(finishing_up)
  {

  free(pairs);

  } //profile

  profiler_dump("profiler.data");

}

