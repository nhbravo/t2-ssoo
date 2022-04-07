#pragma once
#include <stdint.h>

struct process;
typedef struct process Process;

struct process
{
  int32_t pid;
  char *name;
  enum state { RUNNING, READY, WAITING, FINISHED, CREATED } state;
  int32_t init_time;
  int32_t enter_queue_time;
  int32_t running_time;
  int32_t cycles;
  int32_t wait;
  int32_t waiting_delay;
  int32_t s;
  int32_t in_cpu_count;
  int32_t interrupt_count;
  int32_t in_ready_count;
  int32_t in_waiting_count;
};

Process* process_init(
  int32_t pid, char *name, int32_t init_time, int32_t cycles, int32_t wait, int32_t waiting_delay, int32_t s
);
void process_destroy(Process* process);
