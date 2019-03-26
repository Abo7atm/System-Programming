#include "functions.h"
#include <stdio.h>

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
 * randomly generates a
 * maximum of 5 jobs per second
 * and hands them to jod scheduler
 * */
struct Process process_gen()
{
    time_t curtime;
    time(&curtime);

    int p_time, mem, c_date;

    p_time = (rand() % (1000 - 10 + 1)) + 10;
    mem = (rand() % (100 - 1 + 1)) + 1;
    struct Process r = { process_id++, ctime(&curtime), p_time, mem, proba(), proba(), proba(), proba() };
    return r;
}
