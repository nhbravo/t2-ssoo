#pragma once
#include "../process/process.h"

struct queue;
typedef struct queue Queue;

struct queue {
  int process_quantity;
  Process** processes;
  int priority;
  int quantum;
};

Queue* queue_init(int process_quenatity, int priority, int q);
void queue_destroy(Queue* queue);
