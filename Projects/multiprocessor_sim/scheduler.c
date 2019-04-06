#include "functions.h"

/**
 * Schedule jobs and dispatches the job to the first available processor.
 * Checks the resources requirement of the jobs and reserve the jobs required resources before allowing the jobs to run.
 * If resources are not available, the jobs are put to sleep until the scheduler is notified by the memory manager / resource
 * manager about the availability of resources.
 * When a job finishes its total execution time, it is removed from the queue, and from the system.
 * All the resources reserved by this job are released.
 * The scheduler does not implement pre-emtion.
 * */

/**
 * job_queue: all jobs in the system
 * ready_queue: jobs residing in memory
 * waiting_queue: jobs blocked waitng for IO
 * */

#define MAX_MEMORY 2000

Process_queue *job_queue, *ready_queue, *waiting_queue;
int burst = 10, current_memory_usage = 0, IO_load, CPU_load;

void initialize_pqs()
{
    job_queue = init_pq();
    ready_queue = init_pq();
    waiting_queue = init_pq();
}

/* insert new processes into job_queue */
// TODO: return int as an indicator of successful insertion or error.
void insert_job_queue(Process **processes, int n_processes)
{
    Process_node *temp;
    for (int i = 0; i < n_processes; i++)
    {
        enqueue(job_queue, processes[i]);
    }
}

/** 
 * takes jobs from job_queue, insert them into memroy (ready_queue)
 * aka long term scheduler
 * 
 * Initial classification:
 * if process requires more than one IO resource,
 * it's considered an IO bound process,
 * otherwise, it's a cpu bound process. 
 * */
int insert_ready_queue(Process *new_job)
{
    int count_resources; /* count number of resources required */
    int memory_needed = job_queue->head->data->mem;

    if (job_queue->size == 0)
    {
        printf("No jobs available\n");
        return -1;
    }
    else if (current_memory_usage == MAX_MEMORY || memory_needed >= (MAX_MEMORY - current_memory_usage))
    {
        printf("No enough memeory for job\n");
        return -1;
    }
    /**
     * Compare number of IO jobs vs CPU jobs.
     * Initially, if long term scheduler faces a problem with choosing a job,
     * it'll just stop selecting jobs.
     * */
    else if (((IO_load / CPU_load) < 0.7) || ((CPU_load / IO_load) < 0.7))
    {
        printf("Imbalanced jobs\n");
        return -1;
    }

    Process_node *temp = job_queue->head;
    Process *d = temp->data;

    /* count number of resources requried to determine whether IO bound or not */
    count_resources = d->res_a + d->res_b + d->res_c + d->res_d;

    enqueue(ready_queue, new_job);
    current_memory_usage += new_job->mem;

    /* if jo requires 2 or more resources, it is IO bound */
    if (count_resources > 1)
    {
        IO_load++;
    }
    else
    {
        CPU_load++;
    }

    return 0;
}

/* dispatch process to CPU */
void dispatch();

/* check resources the process requires */
int check_resources();

/* if process requires unavailable resources */
int wait();

/* when a process finished all execution */
int remove();

/* handling pre emption */
int interrupt();