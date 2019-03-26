#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int NUM_OF_INT = 0;

/* SIGINT handler */
void no_int()
{
    printf("I SHALL NOT DIE YET!, id: %d\n", getpid());
    NUM_OF_INT++;
    if (NUM_OF_INT >= 4)
    {
        signal(SIGINT, SIG_DFL);
    }
}

/* SIGCHILD signal handler, retrieves data about child's death */
//  int sigaction (int signum, const struct sigaction *restrict action, struct sigaction *restrict old-action)
void post_mortem(int signum, siginfo_t *siginfo)
{
    /*
    SIGCHLD signal is signal 20 in macOS
    */
    printf("Post mortem, code: %d\n", siginfo->si_signo);
    return;
}

int main()
{
    /*  defining sigaction strcut for sigaction() */
    struct sigaction new_action;
    new_action.sa_handler = post_mortem;

    int pid, status, randInt, fd[2], val = 1;

    pipe(fd); /* pipe for communicating between parent and child number 3 */

    for (int i = 1; i < 9; i++)
    {
        if ((pid = fork()) < 0)
        {
            perror("Fork");
        }
        else if (pid == 0)
        {
            switch (i)
            {
            case 1: /* division by zero */
                printf("child number: %d, id: %d\n", i, getpid());
                printf("\t--division by zero result: %d\n", (1 / 0));
                break;
            case 2: /* return random int value and dies normally */
                close(fd[0]); /* child: writing only, close read discriptor */
                randInt = rand();
                if (write(fd[1], &randInt, sizeof(randInt)) < 0)
                {
                    perror("Write:");
                }
                printf("child number: %d, id: %d\n", i, getpid());
                printf("\t--generated random number: %d\n", randInt);
                break;
            case 3: /* loop that tries to write in an initialized pointer */
                printf("child number: %d, id: %d\n", i, getpid());
                for (int j = 0; j < 5; j++)
                {
                    char *a;
                    *a = "this_is_a_very_very_long_string";
                }
                break;
            case 8: /* enter infinite loop */
                printf("child number: %d, this process's pid: %d\n", i, getpid());
                printf("\t--entering infinite loop...\n");
                while (1)
                    ;
                break;
            default:
                break;
            }
            break;
        }
        else
        {
            if (wait(NULL) < 0)
            {
                perror("WAIT");
            }
            /* assigning signal handlers */
            signal(SIGINT, no_int);
            sigaction(SIGCHLD, &new_action, NULL);
            switch (i)
            {
            case 4: /* send signal 2 */
                printf("child number: %d, id: %d\n", i, pid);
                if (kill(pid, 2) != 0)
                {
                    perror("Kill");
                }
                break;
            case 5: /* send signal 9 */
                printf("child number: %d, id: %d\n", i, pid);
                if (kill(pid, 9) != 0)
                {
                    perror("Kill");
                }
                break;
            case 6: /* send signal 14 */
                printf("child number: %d, id: %d\n", i, pid);
                if (kill(pid, 14) != 0)
                {
                    perror("Kill");
                }
                break;
            case 7: /* send signal 15 */
                printf("child number: %d, id: %d\n", i, pid);
                if (kill(pid, 15) != 0)
                {
                    perror("Kill");
                }
                break;
            default:
                break;
            }
            
            // if (waitpid(pid, &status, WUNTRACED) > 0)
            // {
            //     if (WIFSIGNALED(status)) /* if child was signaled */
            //     {
            //         printf("\t--stopped by signal number: %d\n\n", WTERMSIG(status)); /* number of terminating signal */
            //     }
            //     else if (WIFEXITED(status)) /* if the child process terminated normally */
            //     {
            //         if (i == 2)
            //         {
            //             wait(NULL);   /* wait for child to finish writing and exit */
            //             close(fd[1]); /* parent: reading only, close write discriptor */
            //             read(fd[0], &val, sizeof(val));
            //             printf("\t--value return by child: %d\n", val);
            //         }
            //         printf("\t--exited normally with code: %d\n\n", WEXITSTATUS(status)); /* exit code */
            //     }
            //     else
            //     {
            //         printf("\t--some magic happened with process: %d\n\n", i);
            //     }
            // }
            
        }
    }
    return 0;
}
