#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <stdlib.h>
#include <time.h>

extern int process_id;

void prnt(const char* string);
struct Process process_gen();

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

#endif