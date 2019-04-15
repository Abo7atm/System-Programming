#include "functions.h"

void start() /* not init */
{
    initialize_pqs();
    // insert_job_queue(); /* will call generator */
    // insert_ready_queue();
    // printf("Now will run!\n");
    run3();
}

int main()
{
    srand(time(NULL));

    start();
    exit(0);

    Process *ptr, *b;
    Process_queue *queue;
    int size;

    exit(0);
    
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