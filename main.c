#include <stdio.h>
#include <stdlib.h>

#include "parser/pairs_parser.h"
#include "reference_haversine.h"
#include "profiler/profiler.h"


int main(void){


  PROFILE_START(extras);
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

  PROFILE_END(extras);
  PROFILE_DUMP("profiler.data");

}

