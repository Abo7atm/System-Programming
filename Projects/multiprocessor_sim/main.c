#include "functions.h"

int main()
{
    Process *ptr, *b;
    Process_queue *queue;
    int size;

    queue = init_pq();

    // queue = init();

    // printf("size of queue: %d", queue->size);
    // exit(0);

    srand(time(NULL));
    ptr = (Process *)malloc(sizeof(Process) * 5);
    size = generator(queue);

    for (int i = 0; i < size; i++)
    {
        b = dequeue(queue);
        printf("process id: %d, processing time: %d, memory: %d, resources: %d\n",
               b->id, b->p_time, b->mem, b->resources_required);
    }

    return 0;
}