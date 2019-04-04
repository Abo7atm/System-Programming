#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "functions.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

extern int process_id;

void prnt(const char* string);
struct Process process_gen();
int generator(Process_queue *queue);
/* bitwise for res? */
struct
{
    int id;
    char* creation_date;
    int p_time; /* required processing time */
    int mem; /* required memory */
    int res_a; /* requires resource a */
    int res_b; /* requires resource b */
    int res_c; /* requires resource c */
    int res_d; /* requires resource d */
} typedef Process;

/* linkedlist part */

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

Process_queue *init_pq();

void enqueue(Process_queue *queue, Process *x);
void traverse(Process_node *start);
void dequeue(Process_queue *queue);

#endif