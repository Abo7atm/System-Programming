#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "functions.h"
#include <time.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    printf("Hello, padre\n--------\n");

    int open_log_file, run_time, read_bytes;
    if ((open_log_file = open("./cpu_log", O_WRONLY | O_CREAT | O_APPEND, 0644)) < 0)
    {
        perror("open");
        exit(1);
    }

    printf("Processor starting...\n");
    // set log file as stdout
    if (dup2(open_log_file, 1) < 0)
    {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    Process *buf;
    buf = (Process *)malloc(sizeof(Process));

    for (int i = 0; i < 10; i++)
    {
        if ((read_bytes = read(STDIN_FILENO, buf, sizeof(Process))) < 0)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        run_time = (rand() % 10) + 1; /* random number between 1 and 10 */

        /* for verbosity */
        /* printf() will now write to file */

        printf("Action: EXECUTE\t| Process: %d\t| Exec Time: %d\t| id: %d\n", buf->id, buf->p_time, getpid());

        buf->p_time -= run_time;

        usleep(buf->p_time * 1000);

        if (write(3, buf, sizeof(Process)) < 0)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }

        kill(getppid(), SIGUSR1); // "Hey scheduler, I've written to the pipe."
        i++;
    }
    return 0;
}