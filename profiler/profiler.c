#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "profiler.h"

#define PROFILER_SIZE 4
static profiler_t profiler = {0};

static profiler_t profiler_init(size_t size){
  if (size == 0) size = 2;

  record_t *start = malloc(sizeof(record_t) * size);
  if(!start) return (profiler_t){0};

  profiler_t tmp_profiler = {
      .start = start,
      .count = 0,
      .capacity = size,
  };

  return tmp_profiler;
}


static void profiler_push(profiler_t *profiler, record_t record)
{
  if (profiler->count == profiler->capacity){
    profiler->capacity *= 2;
    record_t *new_start = realloc(profiler->start, 
                              sizeof(record_t) * profiler->capacity);
    if (!new_start) return;
    profiler->start = new_start;
  }

  profiler->start[profiler->count] = record;
  profiler->count++;
  return;
}

static void profiler_free(profiler_t *profiler)
{
  free(profiler->start);

  profiler->start = NULL;
  profiler->count = 0;
  profiler->capacity = 0;

  return;
}

    

void profile_record(char *name, unsigned long long cycles)
{
  if (profiler.start == NULL){
    profiler = profiler_init(PROFILER_SIZE);
  }

  for(size_t i = 0; i < profiler.count; i++){
    if(!strcmp(name, profiler.start[i].name)){
      profiler.start[i].calls++;
      profiler.start[i].cycles += cycles;
      return;
    }
  }

  record_t new_record = (record_t){.name = name, .cycles = cycles, .calls = 1};
  profiler_push(&profiler, new_record);

}

void profile_dump(char *file_name){

    FILE *file_ptr;
    file_ptr = fopen(file_name, "w");
    if (file_ptr == NULL) return;

    fprintf(file_ptr, "ESTIMATED CPU CLOCK: %llu\n\n", get_cpu_freq());
    for(size_t i = 0; i < profiler.count; i++){
      fprintf(
        file_ptr,
        "%s:\n\tCYCLES: %llu\n\tCALLS: %zu\n\tAVG_CYCLES: %.2f\n\n",
        profiler.start[i].name,
        profiler.start[i].cycles,
        profiler.start[i].calls,
        (double)profiler.start[i].cycles / profiler.start[i].calls
      );
    }

    fclose(file_ptr);
    profiler_free(&profiler);
}
