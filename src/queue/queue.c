#include <stdlib.h>

#include "queue.h"

Queue* queue_init(int process_quantity, int priority, int q)
{
    Queue* queue = malloc(sizeof(Queue));

    queue -> process_quantity = process_quantity;
    queue -> processes = malloc(process_quantity * sizeof(Process*));
    queue -> priority = priority;
    queue -> quantum = q * priority;

    return queue;
}

void queue_destroy(Queue* queue)
{
    for (int i = 0; i < queue -> process_quantity; i++)
    {
        process_destroy(queue -> processes[i]);
    }
    free(queue -> processes);
    free(queue); 
}
