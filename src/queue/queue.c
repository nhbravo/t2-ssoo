#include <stdlib.h>

#include "queue.h"

Queue *queue_init(int process_quantity, int priority, int q, int total_processes)
{
    Queue *queue = malloc(sizeof(Queue));

    queue -> process_quantity = process_quantity;
    queue -> processes = malloc(total_processes * sizeof(Process *));
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

static void swap(Queue *queue, int i1, int i2)
{
  Process *aux = queue -> processes[i1];
  queue -> processes[i1] = queue -> processes[i2];
  queue -> processes[i2] = aux;
}

static int key(Queue *queue, int i)
{
    if (queue -> priority)
    {
        return queue -> processes[i] -> enter_queue_time;
    }
    // TODO: change to min process length
    return queue -> processes[i] -> enter_queue_time;
}

static void sift_up(Queue *queue, int pos)
{
    if (!pos)
        return;

    int father = (pos - 1) / 2;
    if (key(queue, father) > key(queue, pos))
    {
        swap(queue, pos, father);
        sift_up(queue, father);
    }
}

static void sift_down(Queue *queue, int pos)
{
    int left = pos * 2 + 1;
    int right = left + 1;

    if (queue -> process_quantity <= left)
        return;

    int smaller;
    if (queue -> process_quantity == right || key(queue, left) < key(queue, right))
        smaller = left;
    else
        smaller = right;

    if (key(queue, pos) > key(queue, smaller))
    {
        swap(queue, pos, smaller);
        sift_down(queue, smaller);
    }
}

void process_insert(Queue *queue, Process *process)
{
    queue -> processes[queue -> process_quantity] = process;
    queue -> process_quantity += 1;
    sift_up(queue, queue -> process_quantity - 1);
}

Process *process_pop(Queue *queue)
{
    if (!queue -> process_quantity)
        return NULL;

    Process *process = queue -> processes[0];
    queue -> process_quantity -= 1;
    if (queue -> process_quantity > 0)
    {
        queue -> processes[0] = queue -> processes[queue -> process_quantity];
        queue -> processes[queue -> process_quantity] = NULL;
        sift_down(queue, 0);
    }
    else
    {
        queue -> processes[0] = NULL;
    }

    return process;
}

Process *process_pop_index(Queue *queue, int index)
{
    if (!queue -> process_quantity)
        return NULL;

    Process *process = queue -> processes[index];
    queue -> process_quantity -= 1;
    if (queue -> process_quantity > 0)
    {
        queue -> processes[index] = queue -> processes[queue -> process_quantity];
        queue -> processes[queue -> process_quantity] = NULL;
        int father = (index - 1) / 2;
        if (father >= 0) {
            if (key(queue, father) > key(queue, index)) {
                sift_up(queue, index);
            } else {
                sift_down(queue, index);
            }
        } else {
            sift_down(queue, index);
        }
    }
    else
    {
        queue -> processes[index] = NULL;
    }

    return process;
}
