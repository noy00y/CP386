/*
-------------------------------------------------
First Come First Serve Algorithm Implementation
Author: khan3300@mylaurier.ca
ID: 200143300
-------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Defined constants for now (TODO: Make dynamic)
#define MAX_LINE_LENGTH 500 
#define MAX_TASKS_LENGTH 1000

// data structure used for storing the details of each thread (added completion time)
struct threadInfo{
    int p_id;
    int arr_time;
    int burst_time;
    int waiting_time;
    int turn_around_time;
    int completion_time;
};

void sort(struct threadInfo arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        struct threadInfo key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j].arr_time > key.arr_time) {
            arr[j + 1] = arr[j];j = j - 1;
        } arr[j + 1] = key;
    }
    return;
}

int main(){
    // driver variables
    struct threadInfo tasks[MAX_TASKS_LENGTH]; // create a array to hold the tasks
    int n_tasks=0; // used to keep track of the total number of tasks in the text file and the array
    float average_waiting_time=0.0;
    float average_turn_around_time=0.0;

    // code to process the contents of the file and load it into the "tasks" array: (use A1 code so you do everything at a system level, but for now leave it as is)
    FILE *fp; // file pointer to access the input text file
    char buffer[MAX_LINE_LENGTH]; // buffer for processing lines

    fp = fopen("sample_in_schedule.txt","r"); // open file
    if (fp==NULL){return 1;} // exit if open failed
    while (fgets(buffer, MAX_LINE_LENGTH, fp)!=NULL){ // else get line
        struct threadInfo curr; // create threadInfo locally for current task
        int total_read; // variable for error handling
        total_read = sscanf(buffer, "%i, %i, %i", &curr.p_id, &curr.arr_time, &curr.burst_time);
        if (total_read==3){ // if it was a perfect read
            tasks[n_tasks]=curr; // add local threadInfo to the array
            n_tasks++; // increment total number of tasks
        }
    } fclose(fp); // close file after reading successfuly


    // Handling the non-premptive FCFS algorithm:

    // move tasks to a new (more accurate size - alternative to slicing) and sort it using the insertation sort algorithm
    struct threadInfo tasks_sliced[n_tasks];
    for (int i=0; i<n_tasks; i++){tasks_sliced[i] = tasks[i];} sort(tasks_sliced, n_tasks);


    // print the col header
    printf("Thread ID       Arrival Time    Burst Time  Completion Time Turn-Around Time    Waiting Time\n");
    // perform required computations per task and output it to the terminal
    for (int i=0; i<n_tasks; i++){
        
        // calculate and set the current threads the completion time
        if (i==0 && tasks_sliced[i].arr_time==0){tasks_sliced[i].completion_time=tasks_sliced[i].burst_time;}
        else {tasks_sliced[i].completion_time = tasks_sliced[i-1].completion_time + tasks_sliced[i].burst_time;}

        // calculate and set the current threads turn-around and waiting time
        tasks_sliced[i].turn_around_time = tasks_sliced[i].completion_time - tasks_sliced[i].arr_time;
        tasks_sliced[i].waiting_time = tasks_sliced[i].turn_around_time - tasks_sliced[i].burst_time;
        
        // increase sum for average_waiting and turn-around time
        average_turn_around_time+=tasks_sliced[i].turn_around_time;
        average_waiting_time+=tasks_sliced[i].waiting_time;
        
        // print current p_id's information
        printf("%i\t\t\t%i\t\t%i\t\t%i\t\t%i\t\t%i\n", 
            tasks_sliced[i].p_id, 
            tasks_sliced[i].arr_time, 
            tasks_sliced[i].burst_time,
            tasks_sliced[i].completion_time,
            tasks_sliced[i].turn_around_time,
            tasks_sliced[i].waiting_time);

    }

    // print the averages
    printf("The average waiting time: %.2f\n", average_waiting_time/n_tasks);
    printf("The average turn-around time: %.2f\n", average_turn_around_time/n_tasks);
    return 0;
}