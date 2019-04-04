#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

//Function to return the average wait time of the system
float avg_wait_time(int wt[], int n)
{
    float x = 0;
    int i, sum = 0;
    for (i = 0; i < n; i++)
        sum = sum + wt[i];
    x = sum * 1.0;
    x = x / n;
    return x;
}

//Function to return the average turnaround time of the system
float avg_turnaround_time(int tat[], int n)
{
    float x = 0;
    int i, sum = 0;
    for (i = 0; i < n; i++)
        sum = sum + tat[i];
    x = sum * 1.0;
    x = x / n;
    return x;
}

//Function to fill the gantt chart
//Index is used as the instance of time in the array and the data number at that index represents the process executing at that time
void fill_gantt(int gantt[], int from, int to, int process)
{
    int i;
    for (i = from; i <= to; i++)
        gantt[i] = process;
}

//After a process has executed it's burst cycle, the process queue is rearranged
//The first process is queued at the end and the next process is to be executed
//This function shifts the array elements to 1 position left and attached the first element at the last index
void rearrange_process_queue(int pq[], int n, int running_processes)
{
    int i;
    int temp = pq[0];
    for (i = 0; i < running_processes; i++)
    {
        pq[i] = pq[i + 1];
    }
    pq[running_processes - 1] = temp;
}

int main()
{
    int count, i, j;     //Counter variables
    int time_quantum, n; //Time quantum and number of processes
    int time = 0;        //Variable to keep record of time
    int t_check = 0;     //keeps track of the time
    FILE *fp;
    printf("\t--------------------------------------------------\n");
    printf("\t\tROUND ROBIN SCHEDULING ALGORITHM\n");
    printf("\t--------------------------------------------------\n");
    //Reading data from the file
    fp = fopen("data.txt", "r");
    if (fp == NULL)
    {
        printf("File could not be accessed!\n");
        return 0;
    }
    fscanf(fp, "%d\n%d", &n, &time_quantum);
    //Check if the variables are valid
    //Time quantum and number of processes must be natural numbers
    if (n <= 0 || time_quantum <= 0)
    {
        printf("Invalid data!");
        return 0;
    }
    printf("The number of processes are set to: %d\nThe time quantum is set to:%d\n", n, time_quantum);
    //Arrays to keep track of states
    //at - arrival time, bt - burst time, rt - remaining time, pq - process queue, pflag - denotes wether the process has arrived, tat - turnaround time, wt - wait time
    //The index of each array denotes the (Process index - 1) except pq
    int at[10], bt[10], rt[10], pq[10], pflag[10], tat[10], wt[10];
    //Initially filling all the values as default
    for (j = 0; j < n; j++)
    {
        pq[j] = 0;
        pflag[j] = 0;
    }
    //Scanning other data from the files
    printf("The scanned process data is:\n\n");
    printf("--------------------------------------------------\n");
    printf("|\tINDEX\t|\tAT\t|\tBT\t|\n");
    for (count = 0; count < n; count++)
    {
        fscanf(fp, "%d\t%d", &at[count], &bt[count]);
        printf("|\t%d\t|\t%d\t|\t%d\t|\n", count + 1, at[count], bt[count]);
        rt[count] = bt[count];
        t_check += bt[count];
        //Check for invalid data read
        if (at[count] < 0 || bt[count] <= 0)
        {
            printf("Invalid Data!");
            return 0;
        }
    }
    printf("--------------------------------------------------\n");
    //The index of the gantt variable would represent the time instance
    //The data number at the index would denote the process that was executed at that time
    int gantt[200];
    //Initially filled with default values
    for (i = 0; i < 200; i++)
        gantt[i] = 0;
    //current variable is used to keep track of the time from which a new process started execution
    int current = 0;
    printf("\nThe order of execution:\n\n");
    //Denotes the number of active process in the process queue
    int running_processes = 0;
    //Putting process 1 in the process queue for a start
    pq[0] = 1;
    running_processes = 1;
    pflag[0] = 1;
    int flag = 0;
    //Main loop
    while (running_processes != 0)
    {
        flag = 0; //To represent weather a process has ended execution so it can be removed from the process queue
                  //Printing status of pq and rt so we can see the changes taking place
        printf("Process queue: ");
        for (i = 0; i < running_processes; i++)
            printf("%d ", pq[i]);
        printf("\t");
        printf("Remaining Time: ");
        for (i = 0; i < n; i++)
            printf("%d ", rt[i]);
        printf("\tRunning processes: %d\n", running_processes);
        //Running the first process in queue(at index 0) in the process queue
        //If the remaining time of the process is more than the burst time
        if (rt[pq[0] - 1] > time_quantum)
        {
            rt[pq[0] - 1] = rt[pq[0] - 1] - time_quantum;
            time = time + time_quantum;
            printf("Process %d executed for %d bursts till time %d\n", pq[0], time_quantum, time);
            fill_gantt(gantt, current, time, pq[0]);
            current = time;
        }
        //if the remaining time is less than or equal to the burst time
        //The process will finish it's execution and terminate
        else
        {
            time = time + rt[pq[0] - 1];
            printf("Process %d executed for %d bursts till time %d\n", pq[0], rt[pq[0] - 1], time);
            rt[pq[0] - 1] = 0;
            flag = 1;
            fill_gantt(gantt, current, time, pq[0]);
            current = time;
            //We'll fill the turnaround time and wait time of that process now as it has finished it's execution
            //and the data we need is accessible right now
            tat[pq[0] - 1] = time - at[pq[0] - 1];
            wt[pq[0] - 1] = tat[pq[0] - 1] - bt[pq[0] - 1];
        }
        //Loading the processes that arrived during the time that the first process in pq was executing and adding those processes to the process queue
        for (i = 0; i < n; i++)
        {
            if (at[i] <= time && pflag[i] == 0)
            {
                pq[running_processes] = i + 1;
                running_processes = running_processes + 1;
                pflag[i] = 1;
            }
        }
        //Rearranging the process queue by pushing back all the processes and queuing the first process to the end of the queue
        rearrange_process_queue(pq, n, running_processes);
        //Check if the running process has finished it's execution
        if (flag == 1)
            running_processes = running_processes - 1;
    }
    printf("\n\nExecution Data:\n");
    printf("----------------------------------------------------------------------------------\n");
    printf("|\tProcess\t|\tAT\t|\tBT\t|\tTAT\t|\tWT\t|\n");
    for (i = 0; i < n; i++)
    {
        printf("|\t%d\t|\t%d\t|\t%d\t|\t%d\t|\t%d\t|\n", i + 1, at[i], bt[i], tat[i], wt[i]);
    }
    printf("----------------------------------------------------------------------------------\n");
    printf("\n\nAverage Waiting Time= %f\n", avg_wait_time(wt, n));
    printf("Avg Turnaround Time = %f\n", avg_turnaround_time(tat, n));
    return 0;
}