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
  process -> state = NULL;
  process -> init_time = init_time;
  process -> cycles = cycles;
  process -> wait = wait;
  process -> waiting_delay = waiting_delay;
  process -> s = s;
  return process;
};

void process_destroy(Process* process) {
  free(process -> name);
  free(process);
};
