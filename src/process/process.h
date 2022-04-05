#pragma once
#include <stdint.h>

struct process;
typedef struct process Process;

struct process
{
  int32_t pid;
  char *name;
  int32_t priority;
  enum state { RUNNING, READY, WAITING, FINISHED } state;
  int32_t init_time;
  int32_t current_time;
  int32_t cycles;
  int32_t wait;
  int32_t waiting_delay;
  int32_t s;
};

Process* process_init(
  int32_t pid, char *name, int32_t init_time, int32_t cycles, int32_t wait, int32_t waiting_delay, int32_t s
);
void process_destroy(Process* process);
