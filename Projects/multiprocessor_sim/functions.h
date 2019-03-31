#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "functions.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

extern int process_id;

void prnt(const char* string);
struct Process process_gen();
int generator(struct Process *ptr);
/* bitwise for res? */
struct Process
{
    int id;
    char* creation_date;
    int p_time; /* required processing time */
    int mem; /* required memory */
    int res_a; /* requires resource a */
    int res_b; /* requires resource b */
    int res_c; /* requires resource c */
    int res_d; /* requires resource d */
};

/* linkedlist part */

struct node
{
    struct Process *data;
    struct node *next;
    int count;
};

void insert_at_end(int);
void traverse();
void delete_from_begin();
void delete_from_end();

#endif