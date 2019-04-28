#include "functions.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>


int process_id = 1;

/**
 * probability generator for
 * resources required by process
*/
int proba(int th) /* th => threshold */
{
    float proba = rand() / (float)RAND_MAX * 100.0;
    if (proba >= th)
    {
        return 1;
    }
    return 0;
}

unsigned long get_time()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return 1000000 * t.tv_sec + t.tv_usec;
}

int resources_required()
{
    int resources[] = {RESOURCE_A, RESOURCE_B, RESOURCE_C, RESOURCE_D};

    int result = 0;

    for (int i = 0; i < 4; i++)
    {
        if (proba(90))
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
    r->finished_io = 0;

    // printf("JOB CREATED:\n"), log_fd;
    // printf("\t-- process id: %d, processing time: %d, memory: %d, resources: %d\n",
        // r->id, r->p_time, r->mem, r->resources_required);

    printf("-- Action: CREATE\t| Process: %d\t| Resources: %d\t| P_Time: %d\t| Time: %lu\n", process_id-1, r->resources_required, p_time, get_time());
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
        // printf("Process ID: %d inserted into JOB QUEUE\n", t->id);
        printf("-- Action: INSERT JQ\t| Process: %d\n", t->id);
    }

    return n_processes;
}

void generator2(int fd)
{
    int n_processes = (rand() % (5 + 1));
    Process *t;

    for (int i = 0; i < n_processes + 1; i++)
    {
        t = process_gen();
        if (write(fd, t, sizeof(Process)) < 0)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
        // printf("-- Action: INSERT JQ\t| Process: %d\n", t->id);
    }

    return;
}

int main()
{
    srand(time(NULL));

    char *log_file = "./gen_log";
    char *fifo_path = "/tmp/job_gen_fifo";
    int fifo_fd, counter = 0, log_fd;

    mkfifo(fifo_path, 0666);

    if ((fifo_fd = open(fifo_path, O_WRONLY)) < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    if ((log_fd = open(log_file, O_WRONLY|O_CREAT)) < 0)
    {
        perror("open");
    	exit(EXIT_FAILURE);
    }

    if (dup2(log_fd, STDOUT_FILENO) < 0)
    {
	    perror("dup2");
	    exit(EXIT_FAILURE);
    }

    while(counter < 3)
    {
        // generate 1 to 5 jobs ...
        generator2(fifo_fd);
        // ... every one second
        sleep(1);
        counter++;
    }

    exit(EXIT_SUCCESS);
}
