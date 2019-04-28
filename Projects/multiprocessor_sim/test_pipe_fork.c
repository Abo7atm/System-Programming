#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "functions.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

int piped[2][2], stdout_copy, cpu_fifo_fd, pid[2];

/* recieve signal from cpu indicating finshed execution */
void handle_return();
unsigned long get_time();
void initialize_queue();
void *read_from_gen();
void close_pipes();
void kill_children();

int main()
{
    signal(SIGUSR1, handle_return);
    
    Process_queue ready_queue;
    Process_queue job_queue;

    char *cpu_fifo_path = "/tmp/sched_cpu_fifo";
    char *job_gen_fifo = "/tmp/job_gen_fifo";

    mkfifo(cpu_fifo_path, 0666);
    mkfifo(job_gen_fifo, 0666);

    int gen_pid, wpid, status1, status2, job_gen_fd;
    srand(time(NULL));

    // fork generator
    if ((gen_pid = fork()) < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (gen_pid == 0) // child
    {
        if (execl("./generator", "generator", NULL) < 0)
        {
            printf("generator exec\n");
            perror("EXEC");
        }
    }
    else // parent
    {
        if ((job_gen_fd = open(job_gen_fifo, O_RDONLY)) < 0)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
    }

    // intializing pipes
    // would opening pipes in O_NONBLOCK solve the problem?
    if (pipe(piped[0]) < 0)
    {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }

    if (pipe(piped[1]) < 0)
    {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }

    /* fork child that will exec CPU program */
    for (int i = 0; i < 2; i++)
    {
        if ((pid[i] = fork()) < 0)  // error
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid[i] == 0)       // child process
        {
            // close writing end
            close(piped[i][1]); 

            // set up reading end of pipe for child as STDIN
            if (dup2(piped[i][0], STDIN_FILENO) < 0)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }

            // exeecute 'cpu' program
            if (execl("./chld", "chld") < 0)
            {
                perror("EXEC");
            }
        }
        else                         // parent process
        {
            if (i == 0)
            {
                // if statement to only open FIFO once
                // FIFO to get newly generated jobs
                if ((job_gen_fd = open(job_gen_fifo, O_RDONLY)) < 0)
                {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                // FIFO to get jobs that finished execution
                if ((cpu_fifo_fd = open(cpu_fifo_path, O_RDONLY | O_NONBLOCK)) < 0)
                {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
            }
            close(piped[i][0]); // close reading end of pipe
        }
    }

    // ------ code only executed by the parent ------
    // Why? Because none of this code is executed after running exec in child processes

    Process *process_to_send;
    process_to_send = (Process *) malloc(sizeof(Process));

    int j = 0;

    // dispatching jobs
    // read until FIFO is empty
    while(read(job_gen_fd, process_to_send, sizeof(Process)) > 0)
    {
        // send job to CPU #1
        if (j%2==0) 
        {
            if (write(piped[0][1], process_to_send, sizeof(Process)) < 0)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
        //...send job to CPU #2
        else
        {
            if (write(piped[1][1], process_to_send, sizeof(Process)) < 0)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
        j++;
    }

   // close(job_gen_fd);
   // close_pipes();
   // kill_children();

//    for (int k=0; k<2; k++)
//    {
//        kill(pid[k], SIGUSR2);
//    }
//
//    kill(SIGKILL, gen_pid);
    // to make sure that all the jobs are inserted back to the scheduler
    // while ((wpid = wait(&status)) > 0);

    waitpid(pid[0], &status1, WNOHANG);
    waitpid(pid[1], &status2, WNOHANG);

    printf("finished while loop\n");
    return 0;
} // end of main()

void handle_return()
{
    Process *temp;
    temp = (Process *)malloc(sizeof(Process));
    
    if (read(cpu_fifo_fd, temp, sizeof(Process)) < 0)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }

    printf("Recieved job %d at scheduler at time: %lu, remaining time: %d\n", temp->id, get_time(), temp->p_time);
    return;
}

unsigned long get_time()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return 1000000 * t.tv_sec + t.tv_usec;
}

/*
void initialize_queue()
{
    ready_queue = (Process_queue *)malloc(sizeof(Process_queue));
    job_queue = (Process_queue *)malloc(sizeof(Process_queue));
}
*/

/*
void *read_from_gen()
{
    Process *temp;
    temp = (Process *)malloc(sizeof(Process));

    while(1)
    {

        if (read(job_gen_fd, temp, sizeof(Process)) < 0)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        enqueue(jop_queue, temp);
    }
}
*/

void close_pipes()
{
    close(piped[0][1]);
    close(piped[1][1]);
}

void kill_children()
{
    kill(pid[0], SIGPIPE);
    kill(pid[1], SIGPIPE);
}
