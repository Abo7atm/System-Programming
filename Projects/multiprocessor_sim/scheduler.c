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
int burst, current_memory_usage = 0, IO_load, CPU_load, total_load;
pthread_mutex_t lock;

void update_wait_time(int time_update);

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
    for (int i = 0; i < 10; i++)
    {
        generator(job_queue);
    }
    // printf("job queue size: %d\n", job_queue->size);
    printf("---- Finished Job Generation ----\n\n");
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

    for (int i = 0; i < 20; i++)
    {
        if (job_queue->size == 0)
        {
            printf("No jobs available\n");
            usleep(1000);
            continue;
        }

        Process *new_job = dequeue(job_queue);

        if (current_memory_usage == MAX_MEMORY || memory_needed >= (MAX_MEMORY - current_memory_usage))
        {
            printf("No enough memeory for job\n");
            enqueue(job_queue, new_job);
            usleep(1000);
            continue;
        }

        /**
         * Compare number of IO jobs vs CPU jobs.
         * Initially, if long term scheduler faces a problem with choosing a job,
         * it'll just stop selecting jobs.
         * */
        /* if CPU > 4*IO and incoming job is CPU job, don't insert */
        else if (CPU_load > ((IO_load + 1) * 4) && new_job->resources_required == 0)
        {
            printf("Imbalanced jobs\n");
            enqueue(job_queue, new_job);
            usleep(1000);
            continue;
            // break;
        }
        
        /* if job requires 1 or more resources, it is IO bound */
        if (new_job->resources_required > 0 && check_resource_availablity(new_job->resources_required))
        {
            enqueue(ready_queue, new_job);
            reserve_resources(new_job);
            current_memory_usage += new_job->mem;
            IO_load++;
            // printf("Process ID: %d is IO intensive -- interted in READY QUEUE and resources reserved\n");
        }
        else /* if CPU bound */
        {
            enqueue(ready_queue, new_job);
            current_memory_usage += new_job->mem;
            CPU_load++;
            // printf("Process ID: %d is CPU intensive -- interted in READY QUEUE\n");
        }
        printf("-- Action: INSERT RQ\t| Process: %d\n", new_job->id);
        usleep(10000);
    }
    printf("---- Finished Job Scheduler ----\n\n");
}

/* when a process finished all execution */
void remove_process(Process *finished)
{
    if (finished->resources_required > 0)
    {
        printf("-- Action: RlsRsrc\t| Process: %d\n", finished->id);
        /* release resources */
        release_resource(finished->resources_required);
    }

    /* log this action */
    // some_logging_function(finished);

    /* remove from memory */
    current_memory_usage -= finished->mem;
    free(finished);
}

int dispatch(Process *running)
{
    int run_time;

    printf("-- Action: DISPATCH\t| Process: %d\t| time: %d\n", running->id, running->p_time);

    if (running->resources_required > 0 && !running->finished_io)
    {
        printf("-- Action: EXECUTE\t| Exec Time: %d\n", 1);

        running->p_time--;
        running->wait_time = (rand() % 100) + 1; /* random number between 1 and 100 */

        usleep(1000); /* int nanosleep(const struct timespec *rqtp, struct timespec *rmtp); */
        return -1;
    }
    // printf("dispatch point 2\n");

    /* 80% chance of interrupt */
    if (proba(80))
    {
        run_time = (rand() % 9) + 1; /* random number between 1 and 9 */
    }
    else
    {
        run_time = 10;
    }

    burst = run_time;

    if (run_time > running->p_time)
    {
        run_time = running->p_time;
    }

    printf("-- Action: EXECUTE\t| Exec Time: %d\n", run_time);
    running->p_time -= run_time;

    /* sleep the amout of run_time */
    usleep(run_time * 1000);

    return running->p_time;
}

void run2()
{
    printf("Ayyyyyyyye runniingggggg!\n");
    int num_processors = 2;

    pthread_t thread1, thread2;
    pthread_mutex_init(&lock, NULL);

    printf("thread1 create result: %d\n", pthread_create(&thread1, NULL, round_robin, NULL));
    printf("thread2 create result: %d\n", pthread_create(&thread2, NULL, round_robin, NULL));
    printf("threads has been created and finished running\n");
    pthread_mutex_destroy(&lock);
}

void run3()
{
    round_robin();
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
void *round_robin()
{
    printf("---- Start Round Robin ----\n");
    for (int i = 0; i < 40; i++)
    {
        int dispatch_result; /* value return by dispatch() indicating what next action should be */
        Process *process_to_run;
        process_to_run = dequeue(ready_queue);

        dispatch_result = dispatch(process_to_run);
        
        /* ISSUE: IO Processes return 0 | FIXED */
        if (!dispatch_result) /* if job has finished execution */
        {
            printf("-- Action: REMOVE\t| Process: %d\n", process_to_run->id);
            remove_process(process_to_run);
        }
        else if (dispatch_result < 0) /* if job requested IO */
        {
            printf("-- Action: INSERT WQ\t| Process: %d\t| Wait Time: %d\n", process_to_run->id, process_to_run->wait_time);
            enqueue(waiting_queue, process_to_run);
        }
        else /* insert back into ready queue */
        {
            printf("-- Action: INSERT RQ\t| Process: %d\t| Time: %d\n", process_to_run->id, process_to_run->p_time);
            enqueue(ready_queue, process_to_run);
        }
        update_wait_time(burst);
        printf("\n");
    }
    printf("---- Finish Round Robin ----\n\n");
}

/* ISSUE: IO Processes will always run 1ms at a time. | FIXED */
void update_wait_time(int time_update)
{
    printf("");
    // printf("WQ Size: %d\n", waiting_queue->size);

    if (waiting_queue->size == 0)
    {
        return;
    }

    Process_node *temp;
    temp = waiting_queue->head;

    printf("-- Action: UWT|\t Update Amount: %d\n", time_update);

    while (temp != NULL)
    {
        printf("-- Action: Check WT\t| Process: %d\t| WT: %d\n", temp->data->id, temp->data->wait_time);

        if (time_update >= temp->data->wait_time)
        {
            temp->data->wait_time = 0;
            if (remove_from_middle(waiting_queue, temp->data->id) == 0)
            {
                printf("Removed process: %d\n", temp->data->id);
            }
        }
        else
        {
            temp->data->wait_time -= time_update;
        }

        printf("-- Action: check remaining wait time\t| Process: %d\t| RT: %d\n", temp->data->id, temp->data->wait_time);

        if (temp->data->wait_time <= 0) /* if job has finished waiting */
        {
            printf("\tProcess: %d has finished waiting ", temp->data->id);
            printf("Process has finished IO: %d\n", temp->data->finished_io);
            temp->data->finished_io = 1;
            enqueue(ready_queue, temp->data);
            printf("-- Action: INSERT RQ2\t| Process: %d\n", temp->data->id);
        }

        temp = temp->next;
    }
}

/* Mid term scheduler */
void wait_queue_handler()
{
    while (1)
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
        // update_wait_time();
        sleep(1 / 1000);
    }
}