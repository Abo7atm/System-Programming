#include "functions.h"

/*
 * Schedule jobs and dispatches the job to the first available processor.
 * Checks the resources requirement of the jobs and reserve the jobs required resources before allowing the jobs to run.
 * If resources are not available, the jobs are put to sleep until the scheduler is notified by the memory manager / resource
 * manager about the availability of resources.
 * When a job finishes its total execution time, it is removed from the queue, and from the system.
 * All the resources reserved by this job are released.
 * The scheduler does not implement pre-emtion.
 * 
 * */

/**
 * job_queue: all jobs in the system
 * ready_queue: jobs residing in memory
 * waiting_queue: jobs blocked waitng for IO
 * */

#define MAX_MEMORY = 2000;

Process_queue *job_queue, *ready_queue, *waiting_queue;
int burst = 10, current_memory_usage = 0, IO_load, CPU_load;

void initialize_pqs()
{
    job_queue = init_pq();
    ready_queue = init_pq();
    waiting_queue = init_pq();
}
// Probably no need for this function, as enqueue handles allocation memory for process_node
// Process_node* init_process_node(Process *process)
// {
//     Process_node *result = (Process_node *) malloc(sizeof(Process_node));
//     result->data = process;
//     result->next = NULL;

//     return result;
// }

/* insert new processes into job_queue */
void insert_job_queue(Process **processes, int n_processes)
{
    Process_node *temp;
    for (int i = 0; i < n_processes; i++)
    {
        enqueue(job_queue, processes[i]);
    }
}

/* takes jobs from job_queue, insert them into memroy (ready_queue) */
/* aka long term scheduler */
void insert_ready_queue()
{
    /** categorize processes into either 1) CPU bound, or 2) IO bound
     * initially, if process requires one IO resource,
     * it'll be considred as IO bound
     * */
    if (job_queue->size == 0)
    {
        printf("No jobs available\n");
        return;
    }

    Process_node *temp = job_queue->head;

    while(temp->next != NULL)
    {
        
    }
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