#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "functions.h"
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

void signal_handle();

int main(int argc, char *argv[])
{
    signal(SIGUSR2, signal_handle);
    int open_log_file, run_time, read_bytes, fifo_fd;

    printf("-- -- Processor %d starting...\n", getpid());

    char *cpu_fifo_path = "/tmp/sched_cpu_fifo";
    mkfifo(cpu_fifo_path, 0666);

    // files and fifo preperation
    if ((open_log_file = open("./cpu_log", O_WRONLY | O_CREAT | O_APPEND, 0644)) < 0)
    {
        perror("open");
        exit(1);
    }
   
    // open FIFO  
    if((fifo_fd = open(cpu_fifo_path, O_WRONLY | O_NONBLOCK)) < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // set log file as stdout
    if (dup2(open_log_file, STDOUT_FILENO) < 0)
    {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    // buffer to store job read from pipe
    Process *buf;
    buf = (Process *)malloc(sizeof(Process));
     
    while(read(STDIN_FILENO, buf, sizeof(Process)) > 0)
    {
   	// execution time	
	run_time = (rand() % 10) + 1; // random number between 1 and 10

        // printf writes to log file
        printf("Action: EXECUTE\t| Process: %d\t| Exec Time: %d\t| id: %d\n", buf->id, buf->p_time, getpid());
	
	// adjust job's remaining execution time
        buf->p_time -= run_time;
	
	// sleep to mimic processing time
        usleep(buf->p_time * 1000);
	
	// 'return' the job to scheduler by writing to FIFO
        if (write(fifo_fd, buf, sizeof(Process)) < 0)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
	
	// signal scheduler: "I have finished execution"
        kill(getppid(), SIGUSR1); 
    }
    printf("finish processor %d!\n", getpid());
    exit(EXIT_SUCCESS);
}

void signal_handle()
{
    exit(EXIT_SUCCESS);
}
