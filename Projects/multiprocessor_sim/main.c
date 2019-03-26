#include "functions.h"
#include <stdio.h>

int main()
{
    srand(time(NULL));

    struct Process b;
    for (int i=0; i<10; i++)
    {
        b = process_gen();
        printf("process id: %d, processing time: %d, memory: %d, %d%d%d%d\n",
        b.id, b.p_time, b.mem, b.res_a, b.res_b, b.res_c, b.res_d);
    }
    return 0;
}