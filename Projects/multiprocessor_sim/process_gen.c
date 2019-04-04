#include "functions.h"

int process_id = 1;

/**
 * probability generator for
 * resources required by process
*/
float proba()
{
    float proba = rand() / (float)RAND_MAX * 100.0;
    if (proba > 90)
    {
        return 1;
    }
    return 0;
}

/**
 * Generates a process
 * */
Process process_gen()
{
    time_t curtime;
    time(&curtime);

    int p_time, mem, c_date;

    p_time = (rand() % (1000 - 10 + 1)) + 10;
    mem = (rand() % (100 - 1 + 1)) + 1;
    Process r = { process_id++, ctime(&curtime), p_time, mem, proba(), proba(), proba(), proba() };
    return r;
}

/**
* Generate an array of maximum of five processes
* per second and enqueue them to the parameter queue
*/
int generator(Process_queue *queue)
{
    int n_processes = rand() % (5 + 1);
    Process t;

    for (int i = 0; i < n_processes + 1; i++)
    {
        t = process_gen();
        enqueue(queue, &t);
    }

    return n_processes;
}
