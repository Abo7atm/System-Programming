#include "functions.h"

int main()
{
    struct Process *ptr, b;
    int size;

    srand(time(NULL));
    ptr = (struct Process *)malloc(sizeof(struct Process) * 5);
    size = generator(ptr);

    for (int i = 0; i < size; i++)
    {

        b = ptr[i];
        printf("process id: %d, processing time: %d, memory: %d, %d%d%d%d\n",
               b.id, b.p_time, b.mem, b.res_a, b.res_b, b.res_c, b.res_d);
    }
    return 0;
}