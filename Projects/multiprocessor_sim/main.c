#include "functions.h"

int main()
{
    Process *ptr, b;
    Process_queue *queue;
    int size;

    queue = init();

    printf("size of queue: %d", queue->size);
    exit(0);

    srand(time(NULL));
    ptr = (Process *)malloc(sizeof(Process) * 5);
    size = generator(queue);

    for (int i = 0; i < size; i++)
    {

        b = ptr[i];
        printf("process id: %d, processing time: %d, memory: %d, %d%d%d%d\n",
               b.id, b.p_time, b.mem, b.res_a, b.res_b, b.res_c, b.res_d);
    }

    return 0;
}