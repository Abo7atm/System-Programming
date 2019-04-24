#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "functions.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int piped[2][2], fifo_fd, stdout_copy;

/* recieve signal from cpu indicating finshed execution */
void handle_signal();

int main()
{
    signal(SIGUSR1, handle_signal);

    char *fifo_path = "/tmp/sched_cpu_fifo";

    if (mkfifo(fifo_path, 0666) < 0)
    {
        perror("mkfifo");
        printf("Maybe its because fifo exits\n");
    }

    int pid[2], wpid, status = 0;
    srand(time(NULL));

    /* safety checks */
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
        if ((pid[i] = fork()) < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid[i] == 0) // child process
        {

            printf("orthopedicssssssss\n\n");
            close(piped[i][1]); // close writing end

            int fifo_df_child;
            if ((fifo_df_child = open(fifo_path, O_WRONLY)) < 0)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }

            // pipe to read from
            if (dup2(piped[i][0], STDIN_FILENO) < 0)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }

            // set FIFO to write in as fd = 3
            // TODO: create constants for FDs
            if (dup2(fifo_df_child, 3) < 0)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }

            /* 'cpu' program */
            execl("./chld", "chld"); /* after exec, child stops using this code */
        }
        else // parent process
        {
            if (i == 0)
            {
                // FIFO to read from when signaled
                if ((fifo_fd = open(fifo_path, O_RDONLY)) < 0)
                {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
            }

            close(piped[i][0]); // close reading end of pipe
        }
    }

    Process *process_to_send;
    int j = 0;

    while (j < 10)
    {
        process_to_send = process_gen();
        // send 5 jobs to CPU1 and 5 jobs to CPU2
        if (j < 5)
        {
            if (write(piped[0][1], process_to_send, sizeof(Process)) < 0)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
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

    // to make sure that all the jobs are inserted back to the scheduler
    while ((wpid = wait(&status)) > 0);
    return 0;
}

void handle_signal()
{
    Process *temp;
    temp = (Process *)malloc(sizeof(Process));

    if (read(fifo_fd, temp, sizeof(Process)) < 0)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }

    printf("Recieved job %d at scheduler\n", temp->id);
    // enqueue(ready_queue, temp);
    return;
}