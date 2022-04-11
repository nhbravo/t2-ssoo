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

Queue* queue_init(int process_quenatity, int priority, int q, int total_processes);
void queue_destroy(Queue* queue);
void process_insert(Queue *queue, Process *process);
void process_pop(Queue *queue, int index);
void queue_merge_sort(Queue *queue, int l, int r);
