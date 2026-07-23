#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "profiler.h"

#define PROFILER_SIZE_STARTUP 4
#define SCOPE_STACK_LIMIT 1024

static profiler_t profiler = {0};

static recorder_scope_t scope_stack[SCOPE_STACK_LIMIT];
static size_t scope_stack_counter;



void start_profile_scope(const char *name)
{
  if(scope_stack_counter >= SCOPE_STACK_LIMIT) return;

  scope_stack[scope_stack_counter] = (recorder_scope_t)
                                     {.cycles_start = read_cpu_clock(),
                                      .cycles_child = 0,
                                      .name = name};
  scope_stack_counter++;

}

void end_profile_scope(const char *name)
{
  if(scope_stack_counter == 0)
    return;


  recorder_scope_t *current = &scope_stack[scope_stack_counter - 1];

  if(strcmp(current->name, name) != 0){
    fprintf(stderr,
            "PROFILE ERROR: expected '%s', got '%s'\n",
            current->name,
            name);
    return;
  }

  unsigned long long elapsed = read_cpu_clock() - current->cycles_start;
  unsigned long long exclusive = elapsed - current->cycles_child;

  profile_record(current->name, exclusive);


  scope_stack_counter--;
  if(scope_stack_counter > 0){
    scope_stack[scope_stack_counter - 1].cycles_child += elapsed;
  }

}


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


static bool profiler_push(profiler_t *profiler, record_t record)
{
  if (profiler->count == profiler->capacity){

    profiler->capacity *= 2;
    profiler->capacity = profiler->capacity == 0 ? 2 : profiler->capacity;

    record_t *new_start = realloc(profiler->start, 
                              sizeof(record_t) * profiler->capacity);
    if (!new_start) return false;
    profiler->start = new_start;
  }

  profiler->start[profiler->count] = record;
  profiler->count++;
  return true;
}

static void profiler_free(profiler_t *profiler)
{
  free(profiler->start);

  profiler->start = NULL;
  profiler->count = 0;
  profiler->capacity = 0;

  return;
}

    

bool profile_record(const char *name, unsigned long long cycles)
{
  if (profiler.start == NULL){
    profiler = profiler_init(PROFILER_SIZE_STARTUP);
  }

  for(size_t i = 0; i < profiler.count; i++){

    if(!strcmp(name, profiler.start[i].name)){

      profiler.start[i].calls++;
      profiler.start[i].cycles += cycles;

      return true;
    }
  }

  record_t new_record = {.name = name, 
                           .cycles = cycles, 
                           .calls = 1};

  return profiler_push(&profiler, new_record);
}



bool profiler_dump(const char *file_name){

    FILE *file_ptr;
    file_ptr = fopen(file_name, "w");
    if (file_ptr == NULL) return false;

    unsigned long long total_cycles = 0;
    for(size_t i = 0; i < profiler.count; i++){
      total_cycles += profiler.start[i].cycles;
    }


    unsigned long long cpu_freq = get_cpu_freq();
    fprintf(file_ptr, "ESTIMATED CPU CLOCK: %llu Hz\n", cpu_freq);
    fprintf(file_ptr, "TOTAL CYCLES: %llu (100%%)\n\n", total_cycles);


    for(size_t i = 0; i < profiler.count; i++){
        fprintf(
          file_ptr,
          "%s[%zu]: %llu (%.2f%%)\n",
          profiler.start[i].name,
          profiler.start[i].calls,
          profiler.start[i].cycles,
          ((double)profiler.start[i].cycles / total_cycles) * 100
        );
    }

    fclose(file_ptr);
    profiler_free(&profiler);
    return true;
}
