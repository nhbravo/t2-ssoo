#include "process.h"
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

Process* process_init(
  int32_t pid, char *name, int32_t init_time, int32_t cycles, int32_t wait, int32_t waiting_delay, int32_t s
) {
  Process *process = malloc(sizeof(Process));
  process -> pid = pid;
  process -> name = strdup(name);
  process -> state = CREATED;
  process -> init_time = init_time;
  process -> enter_queue_time = init_time;
  process -> running_time = 0;
  process -> cycles = cycles;
  process -> wait = wait;
  process -> waiting_delay = waiting_delay;
  process -> s = s;
  process -> in_cpu_count = 0;
  process -> interrupt_count = 0;
  process -> first_execution = 0;
  process -> in_ready_count = 0;
  process -> in_waiting_count = 0;
  process -> last_s = init_time;
  process -> quantum_time = 0;
  return process;
};

void process_destroy(Process* process) {
  free(process -> name);
  free(process);
};
