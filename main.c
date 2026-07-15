#include <stdio.h>
#include <stdlib.h>

#include "pairs_parser.h"
#include "reference_haversine.h"
#include "timer/timer.h"

typedef unsigned long long u64;

int main(void){
  u64 program_start_timer = read_cpu_clock();
  
  pair_t *pairs = NULL;
  size_t count = 0;
  
  u64 parser_start_timer = read_cpu_clock(); 

  if (!load_pairs_file("input/input.json", &pairs, &count)) {
      printf("could not load pairs\n");
      return 1;
  }

  u64 parser_end_timer = read_cpu_clock(); 


  u64 haversine_start_timer = read_cpu_clock();
  for (size_t i = 0; i < count; i++) {
      reference_haversine(pairs[i]);
  }

  u64 haversine_end_timer = read_cpu_clock();


  u64 start_time = parser_start_timer - program_start_timer;
  u64 parsing_time = parser_end_timer - parser_start_timer;
  u64 haverine_time = haversine_end_timer - haversine_start_timer;

  u64 program_end_timer = read_cpu_clock();

  u64 total_time = program_end_timer - program_start_timer;
  u64 ending_time =  program_end_timer - haversine_end_timer;

  printf("\nCPU FREQUENCY: %llu Hz\n", get_cpu_freq());
  printf("TOTAL: %llu Hz (100%%) \n\n", total_time);

  printf("STARTING: %llu Hz (%f%%) \n",
      start_time, (double)start_time/total_time * 100);

  printf("PARSING: %llu Hz (%f%%) \n", 
      parsing_time, (double)parsing_time/total_time * 100);

  printf("HAVERINE: %llu Hz (%f%%) \n",
      haverine_time, (double)haverine_time/total_time * 100);
  printf("ENDING: %llu Hz (%f%%) \n", 
      ending_time, (double)ending_time/total_time * 100);

  free(pairs);
}

