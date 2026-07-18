#define _GNU_SOURCE

#include <time.h>

#if defined(_MSC_VER)
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif

#include "timer.h"

typedef unsigned long long u64;

#define OS_TIMER_FREQUENCY 1000000000ULL

u64 get_cpu_freq(void)
{
  u64 os_start  = read_os_timer();
  u64 cpu_start = read_cpu_clock();

  while (read_os_timer() - os_start < OS_TIMER_FREQUENCY)
      ;

  u64 cpu_end = read_cpu_clock();

  return cpu_end - cpu_start;
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
