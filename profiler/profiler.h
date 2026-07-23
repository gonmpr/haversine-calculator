#pragma once
#include "timer.h"
#include <stdbool.h>

// NOTE: THE TIME REPORTED IS EXCLUSIVE; IF YOU HAVE A BLOCK INSIDE
// OTHER, THE EXTERIOR ONE REPORTS THE TIME EXCLUIDING THE INTERIOR ONE.
// THE STRUCT recorder_scope_t IS JUST FOR THIS FUNCTIONALITY

// NOTE: IF YOU WANT TO INCREASE PERFORMANCE, THE FIRST THING YOU
// SHOULD LOOK UP, IS THE CALLS COUNTER FUNCTIONALITY, IT USES strcmp(),
// WHICH IS O(n), JUST IMPLEMENT A HASH.

// NOTE: PROFILE BLOCK DOESN'T WORK IN BLOCKS 
// WITH return, break OR goto STATEMENTS 

// IF YOU WANT TO DISABLE THE PROFILER,
// ADD #include PROFILER_ENABLED 0

#ifndef PROFILER_ENABLED
#define PROFILER_ENABLED 1
#endif

#if PROFILER_ENABLED

#define PROFILE_START(name) \
  start_profile_scope(#name)

#define PROFILE_END(name) \
  end_profile_scope(#name)

#define PROFILE_BLOCK(name) \
    for(bool __once = (start_profile_scope(#name), true); \
        __once; \
        end_profile_scope(#name), __once = false)

#define PROFILE_DUMP(file) profiler_dump(file)

#else

#define PROFILE_START(name) ((void)0)
#define PROFILE_END(name)   ((void)0)

#define PROFILE_BLOCK(name) \
  for(bool __once = true; __once; __once = false)

#define PROFILE_DUMP(file) ((void)0)

#endif



typedef struct {
  const char *name;
  unsigned long long cycles;
  size_t calls;
} record_t;


typedef struct {
  record_t *start;
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

void start_profile_scope(const char *name);
void end_profile_scope(const char *name);
