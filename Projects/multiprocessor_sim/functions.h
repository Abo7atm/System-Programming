// #ifndef FUNCTIONS_H_INCLUDED
// #define FUNCTIONS_H_INCLUDED

// #include "functions.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/* bitwise for res? */
struct process
{
    int id;
    char *creation_date;
    int p_time;             /* required processing time in milliseconds */
    int mem;                /* required memory */
    int resources_required; /* 4 bit binary created with bitwise or */
    int wait_time;
    int finished_io;        /* flag to flip after job has finished waiting for IO */
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

void enqueue(Process_queue *, Process *);
void traverse(Process_node *);
Process *dequeue(Process_queue *);
int remove_from_middle(Process_queue *, int);


/* resource manager part */
#define RESOURCE_A 0b0001
#define RESOURCE_B 0b0010
#define RESOURCE_C 0b0100
#define RESOURCE_D 0b1000

int check_resource_availablity(int);
void release_resource(int);
void reserve_resources(Process *);


/* Process_gen part */
extern int process_id;
int proba(int);
Process *process_gen();
int generator(Process_queue *);


/* scheduler part */
void *round_robin();
void insert_ready_queue();
void insert_job_queue();
void initialize_pqs();
void run2();
void run3();


// #endif