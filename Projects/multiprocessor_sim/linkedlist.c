#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

Process_queue *init_pq()
{
    Process_queue *pq;
    pq = (Process_queue *)malloc(sizeof(Process_queue));
    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
    return pq;
}

void enqueue(Process_queue *queue, Process* x)
{
    Process_node *t;

    t = (Process_node *)malloc(sizeof(Process_node));
    t->data = x;
    t->next = NULL;

    if (queue->head == NULL)
    {
        // printf("head is NOT NULL anymore\n");
        queue->head = t;
        queue->tail = t;
        queue->size++;
        return;
    }

    queue->tail->next = t; 
    queue->tail = t;
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

Process* dequeue(Process_queue *queue)
{
    /* check if queue is empty */
    if (queue->head == NULL)
    {
        printf("Queue empty\n");
        return NULL;
    }

    Process *result;

    result = queue->head->data;
    queue->head = queue->head->next;

    /* upon successful dequeue decrease size */
    queue->size--;
    return result;
}

int remove_from_middle(Process_queue *queue, int id)
{
    if (queue->size == 0)
    {
        printf("Waiting Queue is Empty\n");
        return -1; /* didn't remove anything */
    }

    Process_node *temp, *current;

    temp = queue->head;
    current = temp;

    while(current != NULL)
    {
        if (temp->data->id == id)
        {
            current->next = temp->next;
            queue->size--;
            return 0;
        }
        else
        {
            current = temp;
            temp = temp->next;
        }
    }
    printf("Didn't Find Matching ID\n");
    return -1; /* didn't find id */
}