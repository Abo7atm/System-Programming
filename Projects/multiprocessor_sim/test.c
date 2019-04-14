#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct
{
    int id;
    int mem;
} typedef Process;

struct
{
    Process *data;
    struct Process_node *next;
} typedef Process_node;

struct
{
	int size;
	Process_node *head;
	Process_node *tail;
} typedef Process_queue;

Process_queue *init_pq()
{
    Process_queue *pq;
    pq = (Process_queue *)malloc(sizeof(Process_queue));
    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
    return pq;
}

int proba()
{
    float proba = rand() / (float)RAND_MAX * 100.0;
    if (proba >= 90)
    {
        return 1;
    }
    return 0;
}

int resources_required()
{
    int res[] = {0b0001, 0b0010, 0b0100, 0b1000};

    int result = 0;

    for (int i=0; i<4; i++)
    {
        if (proba())
        {
            result |= res[i];
        }
    }
    return result;
}

void test_resources_required()
{
    for (int i=0; i<50; i++)
    {
        printf("%d:\tresources required: %d\n", i, resources_required());
    }
}

void loop_continue()
{
    int i=0;
    while (i<10)
    {
        if (i++%2==0)
        {
            continue;
        }
        printf("Odd number\n");
    }
}

void test_if()
{
    int x = 1;
    if (x && 0)
    {
        printf("First IF\n");
    }
    else
    {
        printf("Second IF\n");
    }
    return;
}

int main()
{
    test_if();
    exit(0);

    printf("before\n");
    usleep(2000000);
    printf("after\n");
    exit(0);

    loop_continue();
    exit(0);

    Process wow = { 42, 69 };
    Process *t = &wow;
    // printf("address: %d\n", t);

    exit(0);

    srand(time(NULL));

    test_resources_required();
    exit(0);

    Process_queue *long_term;
    long_term = init_pq();

    // printf("head address: %d\n",
    // long_term->head);
    
    if (long_term->head == NULL)
    {
        printf("magic\n");
    }

    return 0;
}