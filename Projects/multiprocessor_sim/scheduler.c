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

pthread_mutex_t rq_lock, wq_lock, loop_lock, jq_lock; /* ready queue lock, waiting queue lock and loop lock*/
pthread_t cpu_thread[2], insert_jq, insert_rq, update_wait, mem_lock;

void *update_wait_time();

void initialize_pqs()
{
    job_queue = init_pq();
    ready_queue = init_pq();
    waiting_queue = init_pq();
}

/* insert new processes into job_queue */
// TODO: return int as an indicator of successful insertion or error.
void *insert_job_queue()
{
    printf("Job queue size: %d\n", job_queue->size);
    while (1)
    {
        generator(job_queue);
        sleep(10);
    }
    // printf("job queue size: %d\n", job_queue->size);
    // printf("---- Finished Job Generation ----\n\n");
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
void *insert_ready_queue()
{
    int count_resources; /* count number of resources required */
    if (job_queue->size == 0)
    {
        sleep(1);
    }
    int memory_needed = job_queue->head->data->mem;

    while (1)
    {
        if (job_queue->size == 0)
        {
            printf("No jobs available\n");
            sleep(1);
            continue;
        }

        Process *new_job = dequeue(job_queue, "JQ");

        if (current_memory_usage == MAX_MEMORY || memory_needed >= (MAX_MEMORY - current_memory_usage))
        {
            printf("No enough memeory for job\n");

            pthread_mutex_lock(&jq_lock);
            /* ------ critical section start ------ */
            enqueue(job_queue, new_job);
            /* ------ critical section end ------ */
            pthread_mutex_unlock(&jq_lock);

            sleep(1);
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

            pthread_mutex_lock(&jq_lock);
            /* ------ critical section start ------ */
            enqueue(job_queue, new_job);
            /* ------ critical section end ------ */
            pthread_mutex_unlock(&jq_lock);

            sleep(1);
            continue;
            // break;
        }

        /* if job requires 1 or more resources, it is IO bound */
        if (new_job->resources_required > 0)
        {
            if (check_resource_availablity(new_job->resources_required))
            {
                pthread_mutex_lock(&rq_lock);
                /* ------ critical section start ------ */
                enqueue(ready_queue, new_job);
                reserve_resources(new_job);
                current_memory_usage += new_job->mem;
                IO_load++;
                /* ------ critical section end ------ */
                pthread_mutex_unlock(&rq_lock);
            }
            // printf("Process ID: %d is IO intensive -- interted in READY QUEUE and resources reserved\n");
        }
        else /* if CPU bound */
        {
            pthread_mutex_lock(&rq_lock);
            /* ------ critical section start ------ */
            enqueue(ready_queue, new_job);
            current_memory_usage += new_job->mem;
            CPU_load++;
            /* ------ critical section end ------ */
            pthread_mutex_unlock(&rq_lock);
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
    /* if it's IO job, release resources reserved by job */
    if (finished->resources_required > 0)
    {
        printf("-- Action: Release\t| Process: %d\n", finished->id);
        /* release resources */
        /* lock | Why lock here? */
        release_resource(finished->resources_required);
        /* unl0ck */
    }

    /* remove from memory */
    /* lock */
    pthread_mutex_lock(&mem_lock);
    /* ------ critical section start ------ */
    current_memory_usage -= finished->mem;
    pthread_mutex_unlock(&mem_lock);
    /* ------ critical section start ------ */

    free(finished); /* deallocate memroy */
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
        burst = 1;
        usleep(1000); /* int nanosleep(const struct timespec *rqtp, struct timespec *rmtp); */
        return -1;
    }

    /* 70% chance of interrupt */
    if (proba(30))
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

/* this function can be deleted */
void run2()
{
    printf("Ayyyyyyyye runniingggggg!\n");
    int num_processors = 2;

    pthread_t thread1, thread2;
    // pthread_mutex_init(&lock, NULL);

    printf("thread1 create result: %d\n", pthread_create(&thread1, NULL, round_robin, NULL));
    printf("thread2 create result: %d\n", pthread_create(&thread2, NULL, round_robin, NULL));
    printf("threads has been created and finished running\n");
    // pthread_mutex_destroy(&lock);
}

void run3()
{
    /* initialize mutex locks */
    pthread_mutex_init(&jq_lock, NULL);   /* lock on enqueuing to job queue */
    pthread_mutex_init(&rq_lock, NULL);   /* lock on enqueuing to ready queue */
    pthread_mutex_init(&wq_lock, NULL);   /* lock on enqueuing to waiting queue */
    pthread_mutex_init(&loop_lock, NULL); /* lock on loop in update_wait_time() */
    pthread_mutex_init(&mem_lock, NULL);  /* lock on memory usage variable */

    /* create long term scheduler */
    if (pthread_create(&insert_jq, NULL, insert_job_queue, NULL) != 0)
    {
        printf("error creating long term scheduler\n");
    }

    /* create short term scheduler */
    if (pthread_create(&insert_rq, NULL, insert_ready_queue, NULL) != 0)
    {
        printf("error creating short term scheduler\n");
    }

    /* update wait time thread so it can be independent */
    if (pthread_create(&update_wait, NULL, update_wait_time, NULL) != 0)
    {
        printf("error creating awit time updater\n");
    }

    /* creating CPU threads */
    for (int i = 0; i < 2; i++)
    {
        if (pthread_create(&(cpu_thread[i]), NULL, round_robin, NULL) != 0)
        {
            printf("error creating thread\n");
            exit(0);
        }
    }

    pthread_join(insert_jq, NULL);
    pthread_join(insert_rq, NULL);
    pthread_join(update_wait, NULL);
    pthread_join(cpu_thread[0], NULL);
    pthread_join(cpu_thread[1], NULL);

    pthread_mutex_destroy(&jq_lock);
    pthread_mutex_destroy(&rq_lock);
    pthread_mutex_destroy(&wq_lock);
    pthread_mutex_destroy(&loop_lock);
    pthread_mutex_destroy(&mem_lock);

    return;
}

/**
 * round_robin() only interacts with ready_queue
 * Takes process at head of ready_queue, "runs" it by
 * subtracting a random number of unit time, if random = 10, then this
 * the process has exhausted all of it's quantum, else, process has been
 * interrupted.
 * Diffrentiate between IO bound and CPU bound jobs.
 * if (jobs is IO bound):
 *  1) run for 1 ms and generate a random wait time that represents IO wait time.
 *  2) insert to waiting queue
 * else:
 *  run as implemented below
 *  */
void *round_robin()
{
    Process_node *temp;
    int i = 0;

    printf("---- Start Round Robin ----\n");
    while (1)
    // while (i < 100)
    {
        if (ready_queue->size == 0)
        {
            sleep(1);
            continue;
        }

        int dispatch_result; /* value returned by dispatch() indicating what next action should be */
        Process *process_to_run;

        pthread_mutex_lock(&rq_lock);
        /* ------ critical section start ------ */
        process_to_run = dequeue(ready_queue, "RQ");
        /* ------ critical section end ------ */
        pthread_mutex_unlock(&rq_lock);

        dispatch_result = dispatch(process_to_run);

        /* ISSUE: IO Processes return 0 | FIXED */
        if (dispatch_result == 0) /* if job has finished execution */
        {
            printf("-- Action: REMOVE\t| Process: %d\n", process_to_run->id);
            remove_process(process_to_run);
        }
        else if (dispatch_result < 0) /* if job requested IO */
        {
            printf("-- Action: INSERT WQ\t| Process: %d\t| Wait Time: %d\n", process_to_run->id, process_to_run->wait_time);

            pthread_mutex_lock(&wq_lock);
            /* ------ critical section start ------ */
            enqueue(waiting_queue, process_to_run);
            /* ------ critical section end ------ */
            pthread_mutex_unlock(&wq_lock);
        }
        else /* if execition time ramaining > 0, insert back into ready queue */
        {
            printf("-- Action: INSERT RQ\t| Process: %d\t| Time: %d\n", process_to_run->id, process_to_run->p_time);

            pthread_mutex_lock(&rq_lock);
            /* --- critical section start --- */
            enqueue(ready_queue, process_to_run);
            /* ------ critical section end ------ */
            pthread_mutex_unlock(&rq_lock);
        }

        /* debugging: why some jobs are still in the RQ when they have been removed? */

        pthread_mutex_lock(&loop_lock);
        /* ------ critical section start ------ */
        printf("-- Action: CHECK RQ\t| JOBS: [");
        temp = ready_queue->head;
        while (temp != NULL)
        {
            if (temp->next == NULL)
            {
                printf("%d", temp->data->id);
            }
            else
            {
                printf("%d, ", temp->data->id);
            }
            temp = temp->next;
        }
        printf("]\n");
        /* ------ critical section end ------ */
        pthread_mutex_unlock(&loop_lock);

        pthread_mutex_lock(&loop_lock);
        /* ------ critical section start ------ */
        temp = waiting_queue->head;

        printf("-- Action: CHECK WQ\t| JOBS: [");
        while (temp != NULL)
        {
            if (temp->next == NULL)
            {
                printf("%d", temp->data->id);
            }
            else
            {
                printf("%d, ", temp->data->id);
            }
            temp = temp->next;
        }
        printf("]\n");
        /* ------ critical section end ------ */
        pthread_mutex_unlock(&loop_lock);

        printf("\n");
        i++;
    }
    printf("---- Finish Round Robin ----\n\n");
}

/* ISSUE: IO Processes will always run 1ms at a time. | FIXED */
void *update_wait_time()
{
     if (waiting_queue->size == 0)
    {
        printf("waiting queue is zero\n");
        usleep(1000); /* nothign to update */
    }

    Process_node *temp; /* pointer used to traveres through waiting queue */
    temp = waiting_queue->head;

    /* traverse over all waiting jobs and subtracting burst time from waiting time */
    while (1)
    {
        if (temp == NULL)
        {
            /* waiting queue is empty */
            usleep(1000);
            if (waiting_queue->size > 0)
            {
                temp = waiting_queue->head;
            }
            continue;
        }
        /* wait time before update */
        printf("-- Action: Check WT\t| Process: %d\t| WT: %d\n", temp->data->id, temp->data->wait_time);

        /* since each iteration in this loop resembels one ms, decrement by one only. */
        temp->data->wait_time--;

        /* remaining time */
        printf("-- Action: Check RT\t| Process: %d\t| RT: %d\n", temp->data->id, temp->data->wait_time);

        if (temp->data->wait_time <= 0) /* if job has finished waiting */
        {
            temp->data->finished_io = 1; /* 'job has finished IO business' FLAG */
            /* remove from waiting queue */
            if (remove_from_middle(waiting_queue, temp->data->id) != 0)
            {
                printf("Problem with removing process id: %d\n", temp->data->id);
            }
            printf("-- Action: REMOVE WQ\t| Process: %d\n", temp->data->id);

            pthread_mutex_lock(&rq_lock);
            /* ------ critical section start ------ */
            enqueue(ready_queue, temp->data);
            /* ------ critical section end ------ */
            pthread_mutex_unlock(&rq_lock);

            printf("-- Action: INSERT RQ2\t| Process: %d\n", temp->data->id);
        }
        temp = temp->next;
    }
}

/* Mid term scheduler */
/* this can be deleted */
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