#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

// void insert_at_end(int);
// void traverse();
// void delete_from_begin();
// void delete_from_end();

void enqueue(struct node *start , struct Process *x)
{
    struct node *t, *temp;

    t = (struct node *)malloc(sizeof(struct node));

    if (start == NULL)
    {
        start = t;
        start->data = x;
        start->next = NULL;
        return;
    }

    temp = start;

    while (temp->next != NULL)
        temp = temp->next;

    temp->next = t;
    t->data = x;
    t->next = NULL;
}

void traverse(struct node *start)
{
    struct node *t;

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

void dequeue(struct node *start)
{
    struct node *t;
    int n;

    if (start == NULL)
    {
        printf("Linked list is already empty.\n");
        return;
    }

    n = start->data;
    t = start->next;
    free(start);
    start = t;

    printf("%d deleted from beginning successfully.\n", n);
}
