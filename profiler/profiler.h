#pragma once
#include "timer.h"
#include <stdbool.h>

// THE TIME REPORTED IS EXCLUSIVE; IF YOU HAVE A BLOCK INSIDE
// OTHER, THE EXTERIOR ONE REPORTS THE TIME EXCLUIDING THE INTERIOR ONE.
// THE STRUCT recorder_scope_t IS JUST FOR THIS FUNCTIONALITY

#define PROFILE_START(name) \
    start_profile_scope(#name)

#define PROFILE_END(name) \
    end_profile_scope(#name)

// NOTE: DOESN'T WORK IN BLOCKS WITH return, break OR goto STATEMENTS 
#define PROFILE_BLOCK(name) \
    for(bool __once = (start_profile_scope(#name), true); \
        __once; \
        end_profile_scope(#name), __once = false)


typedef struct {
  const char *name;
  unsigned long long cycles;
  unsigned long long min;
  unsigned long long max;
  size_t calls;
} recorder_t;


typedef struct {
  recorder_t *start;
  size_t count;
  size_t capacity;
} profiler_t;

typedef struct {
  unsigned long long cycles_start;
  unsigned long long cycles_child;
  const char *name;
} recorder_scope_t;

bool profile_record(const char *name, unsigned long long cycles);
bool profiler_dump(const char *file_name);

void start_profile_scope();
void end_profile_scope();
