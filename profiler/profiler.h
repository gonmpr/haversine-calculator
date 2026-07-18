#pragma once
#include "timer.h"

#define PROFILE_START(name) \
    u64 profile_start_##name = read_cpu_clock()

#define PROFILE_END(name) \
    profile_record(#name, read_cpu_clock() - profile_start_##name)

#define PROFILE_BLOCK(name) \
    for (PROFILE_START(name); profile_start_##name; \
         PROFILE_END(name), profile_start_##name = 0)


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

void profile_record(char *name, unsigned long long cycles);
void profile_dump(char *file_name);
