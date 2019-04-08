#include "functions.h"

/* 0->unused, 1->used */
int resources[] = {0, 0, 0, 0};
/* array of jobs using resources */
int users[] = {0, 0, 0, 0};
/* for bitwise and */
int resources_in_binary[] = {0b0001, 0b0010, 0b0100, 0b1000};

/**
 * create a queue for each resource,
 * and check if the queue is empty, if not, insert to the queue
 * when a resource has become head of queue, notify the scheduler,
 * to take it out of sleep and assign that job to the requested resource.
 * 
 * how to notify scheduler?
 * */

int check_resource_availablity(int resource_required, int pid)
{

    for (int i=0; i<4; i++)
    {
        /* check what resources are requested */
        if (resource_required & resources_in_binary[i])
        {
            /* if resource is being used, nenoted as 1.*/
            if (resources[i])
            {
                printf("resource unavailable!\n");
                return -1;
            }
            else
            {
                /* flag resource as being used */
                resources[i] = 1;
                /* log job id using resource */
                users[i] = pid;
            }
        }
    }
    return 0;
}

int release_resource(int resource_released)
{
    for (int i=0; i<4; i++)
    {
        if (resource_released & resources_in_binary[i])
        {
            /* check if resource is not being used */
            if (resources[i] == 0)
            {
                printf("Resource is already free!\n");
                return -1;
            }
            /* release resource */
            else
            {
                resources[i] = 0;
                users[i] = 0;
                notify_scheduler();
            }
        }
    }
    return 0;
}

void notify_scheduler()
{
    /* signal the scheduler */
    /* I think there should be a lot of signal to achieve the behavior desired. */
}