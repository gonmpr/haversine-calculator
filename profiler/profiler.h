#pragma once
#include "timer.h"

#define PROFILE_BLOCK(name)                                  \
  for (u64 __start = read_cpu_clock();                     \
       __start;                                            \
       profile_record(#name, read_cpu_clock() - __start),  \
       __start = 0)


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
