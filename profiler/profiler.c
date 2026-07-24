#define _GNU_SOURCE
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "profiler.h"

#if defined(_MSC_VER)
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif

//            ## SETTINGS ##

#define OS_TIMER_FREQUENCY 1000000000ULL

#define PROFILER_SIZE_entryUP 4
#define SCOPE_STACK_LIMIT 1024

static profiler_t profiler = {0};

static recorder_scope_t scope_stack[SCOPE_STACK_LIMIT];
static size_t scope_stack_counter;


//          ## EXCLUSIVE TIME STACK ## 

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

  u64 elapsed = read_cpu_clock() - current->cycles_start;
  u64 exclusive = elapsed - current->cycles_child;

  profile_record(current->name, exclusive);


  scope_stack_counter--;
  if(scope_stack_counter > 0){
    scope_stack[scope_stack_counter - 1].cycles_child += elapsed;
  }

}

//            ## PROFILER UTILS ##

static profiler_t profiler_init(size_t size){
  if (size == 0) size = 2;

  record_t *entry = malloc(sizeof(record_t) * size);
  if(!entry) return (profiler_t){0};

  profiler_t tmp_profiler = {
      .entry = entry,
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

    record_t *new_entry = realloc(profiler->entry, 
                              sizeof(record_t) * profiler->capacity);
    if (!new_entry) return false;
    profiler->entry = new_entry;
  }

  profiler->entry[profiler->count] = record;
  profiler->count++;
  return true;
}

static void profiler_free(profiler_t *profiler)
{
  free(profiler->entry);

  profiler->entry = NULL;
  profiler->count = 0;
  profiler->capacity = 0;

  return;
}

    
//            ## PROFILER ##

bool profile_record(const char *name, u64 cycles)
{
  if (profiler.entry == NULL){
    profiler = profiler_init(PROFILER_SIZE_entryUP);
  }

  for(size_t i = 0; i < profiler.count; i++){

    if(!strcmp(name, profiler.entry[i].name)){

      profiler.entry[i].calls++;
      profiler.entry[i].cycles += cycles;

      return true;
    }
  }

  record_t new_record = {.name = name, .cycles = cycles, .calls = 1};
  return profiler_push(&profiler, new_record);
}

bool profiler_dump(const char *file_name){

  FILE *file_ptr;
  file_ptr = fopen(file_name, "w");
  if (file_ptr == NULL) return false;

  u64 total_cycles = 0;
  for(size_t i = 0; i < profiler.count; i++){
    total_cycles += profiler.entry[i].cycles;
  }

  fprintf(file_ptr,
          "\n  ESTIMATED CPU CLOCK: %" PRIu64 " Hz\n", 
          get_cpu_freq());

  fprintf(file_ptr, "  TOTAL CYCLES: %" PRIu64 " (100%%)\n\n", total_cycles);


  for(size_t i = 0; i < profiler.count; i++){
  fprintf(
    file_ptr,
    "  %s[%zu]: %" PRIu64 " (%.2f%%)\n",
    profiler.entry[i].name,
    profiler.entry[i].calls,
    profiler.entry[i].cycles,
    ((double)profiler.entry[i].cycles / total_cycles) * 100
  );
  }
  fprintf(file_ptr, "\n");

  fclose(file_ptr);
  profiler_free(&profiler);
  return true;
}



//            ## TIMER ##

u64 get_cpu_freq(void)
{
  u64 os_entry  = read_os_timer();
  u64 cpu_entry = read_cpu_clock();

  while (read_os_timer() - os_entry < OS_TIMER_FREQUENCY)
      ;

  u64 cpu_end = read_cpu_clock();
  return cpu_end - cpu_entry;
}

u64 read_cpu_clock(void)
{
  return __rdtsc();
}

u64 read_os_timer(void)
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

  return (u64)ts.tv_sec * OS_TIMER_FREQUENCY + ts.tv_nsec;
}
