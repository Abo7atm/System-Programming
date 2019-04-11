#include "functions.h"
#include <pthread.h>

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
 * waiting_queue: jobs waitng for IO
 * */

#define MAX_MEMORY 2000

Process_queue *job_queue, *ready_queue, *waiting_queue;
int burst = 10, current_memory_usage = 0, IO_load, CPU_load, total_load;
pthread_mutex_t lock;

void initialize_pqs()
{
    job_queue = init_pq();
    ready_queue = init_pq();
    waiting_queue = init_pq();
}

/* insert new processes into job_queue */
// TODO: return int as an indicator of successful insertion or error.
void insert_job_queue()
{
    for (int i = 0; i < 5; i++)
    {
        generator(job_queue);
    }
    printf("job queue size: %d\n", job_queue->size);
}

/** 
 * takes jobs from job_queue, insert them into memroy (ready_queue)
 * aka long term scheduler
 * 
 * Initial classification:
 * if process requires one or more IO resources,
 * it's considered an IO bound process,
 * otherwise, it's a cpu bound process. 
 * */
void insert_ready_queue()
{
    int count_resources; /* count number of resources required */
    int memory_needed = job_queue->head->data->mem;

    while (1)
    {
        if (job_queue->size == 0)
        {
            printf("No jobs available\n");
            sleep(1);
            continue;
        }

        Process *new_job = dequeue(job_queue);

        if (current_memory_usage == MAX_MEMORY || memory_needed >= (MAX_MEMORY - current_memory_usage))
        {
            printf("No enough memeory for job\n");
            enqueue(job_queue, new_job);
            sleep(1);
            continue;
        }

        /**
         * Compare number of IO jobs vs CPU jobs.
         * Initially, if long term scheduler faces a problem with choosing a job,
         * it'll just stop selecting jobs.
         * */
        else if (CPU_load > ((IO_load + 1) * 4))
        {
            printf("Imbalanced jobs\n");
            enqueue(job_queue, new_job);
            sleep(1);
            continue;
        }

        /* if job requires 1 or more resources, it is IO bound */
        if (count_resources > 0 && check_resource_availablity(new_job->resources_required))
        {
            enqueue(ready_queue, new_job);
            current_memory_usage += new_job->mem;
            IO_load++;
            printf("Process ID: %d is IO intensive -- interted in READY QUEUE and resources reserved\n");
        }
        else /* if CPU bound */
        {
            enqueue(ready_queue, new_job);
            current_memory_usage += new_job->mem;
            CPU_load++;
            printf("Process ID: %d is CPU intensive -- interted in READY QUEUE\n");
        }
        sleep(1);
    }
}

/* when a process finished all execution */
int remove_process(Process *finished)
{
    /* release resources */
    release_resource(finished->resources_required);

    /* log this action */
    // some_logging_function(finished);

    /* remove from memory */
    current_memory_usage -= finished->mem;
    free(finished);
}

int run_process(Process *running)
{
    int run_time = (rand() % 10) + 10; /* random number between 1 and 10 */
    /* sleep the amout of run_time */
    if (run_time > running->p_time)
    {
        running->p_time = 0;
        return 0;
    }
    running->p_time -= run_time;
    sleep(run_time / 1000);
}

void run2(Process *run_this)
{
    int num_processors = 2;

    pthread_t thread1, thread2;
    pthread_mutex_init(&lock, NULL);

    pthread_create(&thread1, NULL, round_robin, NULL);
    pthread_create(&thread2, NULL, round_robin, NULL);

    pthread_mutex_destroy(&lock);
}

/**
 * round_robin() only interacts with ready_queue
 * Takes process at head of ready_queue, "runs" it by
 * subtracting a random number of unit time, if random = 10, then this
 * the process has exhausted all of it's quantum, else, process has been
 * interrupted.
 * Diffrentiate between IO bound and CPU bound jobs.
 * if (jobs is io bound):
 *  run for 1 ms and generate a random wait time that represents IO wait time.
 * else:
 *  run as implemented below
 *  */
void round_robin()
{
    while (ready_queue->size != 0)
    {
        Process *process_to_run;

        pthread_mutex_lock(&lock);
        process_to_run = dequeue(ready_queue);
        pthread_mutex_unlock(&lock);

        run_process(process_to_run);

        pthread_mutex_lock(&lock);
        enqueue(ready_queue, process_to_run);
        pthread_mutex_unlock(&lock);
    }
}

void update_wait_time()
{
    Process_node *temp;
    temp = waiting_queue->head;

    while (temp->next != NULL)
    {
        temp->data->wait_time--;
    }
}

/* Mid term scheduler */
wait_queue_handler()
{
    while(1)
    {
        if (waiting_queue->size == 0)
        {
            printf("No waiting processes\n");
            sleep(1);
        }
        Process_node *temp = waiting_queue->head;
        while (temp->next != NULL)
        {
            if (temp->data->wait_time == 0)
            {
                /* mutex lock */
                enqueue(ready_queue, temp->data);
                /* mutex unlock */
            }
        }
        /* sleep one tick */
        update_wait_time();
        sleep(1/1000);
    }
}