#include <stdio.h>
#include <stdlib.h>

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

int main()
{
    Process_queue *long_term;
    long_term = init_pq();

    printf("head address: %d\n",
    long_term->head);
    
    if (long_term->head == NULL)
    {
        printf("magic\n");
    }

    return 0;
}