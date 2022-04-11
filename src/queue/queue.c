#include <stdlib.h>
#include <stdio.h>

#include "queue.h"

Queue *queue_init(int process_quantity, int priority, int q, int total_processes)
{
    Queue *queue = malloc(sizeof(Queue));

    queue -> process_quantity = process_quantity;
    queue -> processes = malloc(total_processes * sizeof(Process *));
    for (int i = 0; i < total_processes; i++) {
        queue -> processes[i] = NULL;
    }
    queue -> priority = priority;
    queue -> quantum = q * priority;

    return queue;
}

void queue_destroy(Queue *queue)
{
    for (int i = 0; i < queue -> process_quantity; i++)
    {
        process_destroy(queue -> processes[i]);
    }
    free(queue -> processes);
    free(queue);
}

static int key(Process *process, int priority)
{
    if (priority)
    {
        return process -> enter_queue_time;
    }
    // Cola 3, revisar
    return process -> wait - process -> running_time % process -> wait;
}

void process_insert(Queue *queue, Process *process)
{
    queue -> processes[queue -> process_quantity] = process;
    int new_index = queue -> process_quantity;
    queue -> process_quantity += 1;
    if (queue -> priority == 2) {
        for (int i = queue -> process_quantity - 2; i >= 0; i --) {
            if (key(queue -> processes[i], queue -> priority) == key(queue -> processes[new_index], queue -> priority)) {
                if (
                    queue -> processes[i] -> enter_queue_time == queue -> processes[i] -> init_time
                    && queue -> processes[new_index] -> enter_queue_time != queue -> processes[new_index] -> init_time
                ) {
                    queue -> processes[new_index] = queue -> processes[i];
                    queue -> processes[i] = process;
                    new_index = i;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    }
}

void process_pop(Queue *queue, int index)
{
    queue -> process_quantity -= 1;
    for (int i = index; i < queue -> process_quantity; i++) {
        queue -> processes[i] = queue -> processes[i + 1];
    }
    queue -> processes[queue -> process_quantity] = NULL;
}

// Merge sort adapted from https://www.geeksforgeeks.org/merge-sort/

static void merge(Queue *queue, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
  
    Process *processes_L[n1], *processes_R[n2];
  
    for (i = 0; i < n1; i++)
        processes_L[i] = queue -> processes[l + i];
    for (j = 0; j < n2; j++)
        processes_R[j] = queue -> processes[m + 1 + j];
  
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (key(processes_L[i], queue -> priority) <= key(processes_R[j], queue -> priority)) {
            queue -> processes[k] = processes_L[i];
            i++;
        }
        else {
            queue -> processes[k] = processes_R[j];
            j++;
        }
        k++;
    }
  
    while (i < n1) {
        queue -> processes[k] = processes_L[i];
        i++;
        k++;
    }
  
    while (j < n2) {
        queue -> processes[k] = processes_R[j];
        j++;
        k++;
    }
}

void queue_merge_sort(Queue *queue, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        queue_merge_sort(queue, l, m);
        queue_merge_sort(queue, m + 1, r);
  
        merge(queue, l, m, r);
    }
}
