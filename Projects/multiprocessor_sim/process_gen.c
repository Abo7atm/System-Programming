#include "functions.h"

int process_id = 1;

/**
 * probability generator for
 * resources required by process
*/
int proba()
{
    float proba = rand() / (float)RAND_MAX * 100.0;
    if (proba >= 90)
    {
        return 1;
    }
    return 0;
}

int resources_required()
{
    int resources[] = {RESOURCE_A, RESOURCE_B, RESOURCE_C, RESOURCE_D};

    int result = 0;

    for (int i = 0; i < 4; i++)
    {
        if (proba())
        {
            result |= resources[i];
        }
    }
    return result;
}

/**
 * Generates a process
 * */
Process *process_gen()
{
    time_t curtime;
    time(&curtime);

    int p_time, mem, c_date;

    p_time = (rand() % (1000 - 10 + 1)) + 10; /* random number between 10 and 1000 */
    mem = (rand() % (100 - 1 + 1)) + 1; /* random number between 1 and 100 */

    Process *r = (Process *)malloc(sizeof(Process));

    r->id = process_id++;
    r->creation_date = ctime(&curtime);
    r->p_time = p_time;
    r->mem = mem;
    r->resources_required = resources_required();
    r->wait_time = 0;

    printf("JOB CREATED:\n");
    printf("\t-- process id: %d, processing time: %d, memory: %d, resources: %d\n",
        r->id, r->p_time, r->mem, r->resources_required);

    return r;
}

/**
* Generate an array of maximum of five processes
* per second and enqueue them to the parameter queue
*/
int generator(Process_queue *queue)
{
    int n_processes = (rand() % (5 + 1));
    Process *t;

    for (int i = 0; i < n_processes + 1; i++)
    {
        t = process_gen();
        enqueue(queue, t);
        printf("Process ID: %d inserted into JOB QUEUE\n", t->id);
    }

    return n_processes;
}
