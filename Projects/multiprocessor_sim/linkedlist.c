#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

// void insert_at_end(int);
// void traverse();
// void delete_from_begin();
// void delete_from_end();

Process_queue *init_pq()
{
    Process_queue *pq;
    pq = (Process_queue *)malloc(sizeof(Process_queue));
    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
    return pq;
}

void enqueue(Process_queue *queue, Process *x)
{
    Process_node *t;

    t = (Process_node *)malloc(sizeof(Process_node));
    t->data = x;
    t->next = NULL;

    if (queue->head == NULL)
    {
        queue->head = x;
        queue->tail = queue->head;
        return;
    }

    queue->tail->next = x;
    queue->tail = x;
    /* upon successful enqueue, increase the size */
    queue->size++;
}

void traverse(Process_node *start)
{
    Process_node *t;

    t = start;

    if (t == NULL)
    {
        printf("Linked list is empty.\n");
        return;
    }

    while (t->next != NULL)
    {
        printf("%d\n", t->data);
        t = t->next;
    }

    printf("%d\n", t->data);
}

Process_node* dequeue(Process_queue *queue)
{
    /* check if queue is empty */
    if (queue->head == NULL)
    {
        printf("Queue empty\n");
        return NULL;
    }

    Process_node *result;

    result = queue->head;
    queue->head = queue->head->next;

    /* upon successful dequeue decrease size */
    queue->size--;
    return result;
}
