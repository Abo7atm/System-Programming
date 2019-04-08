// #ifndef FUNCTIONS_H_INCLUDED
// #define FUNCTIONS_H_INCLUDED

// #include "functions.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/* bitwise for res? */
struct process
{
    int id;
    char *creation_date;
    int p_time;             /* required processing time */
    int mem;                /* required memory */
    int resources_required; /* 4 bit binary created with bitwise or */
} typedef Process;


/* linkedlist part */

struct process_node
{
    Process *data;
    struct Process_node *next;
} typedef Process_node;

struct process_queue
{
    int size;
    Process_node *head;
    Process_node *tail;
} typedef Process_queue;

Process_queue *init_pq();

void enqueue(Process_queue *queue, Process *x);
void traverse(Process_node *start);
Process *dequeue(Process_queue *queue);


/* resource manager part */
#define RESOURCE_A 0b0001
#define RESOURCE_B 0b0010
#define RESOURCE_C 0b0100
#define RESOURCE_D 0b1000

int check_resource_availablity(Process *user);
void release_resource(int resource_released);


/* Process_gen part */
extern int process_id;

Process *process_gen();
int generator(Process_queue *queue);


/* scheduler part */
void round_robin();
void insert_ready_queue(Process *new_job);
void insert_job_queue();
void initialize_pqs();


// #endif