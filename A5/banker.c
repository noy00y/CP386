/*
-------------------------------------------------
Bankers Algorithm Implementation
Author: khan3300@mylaurier.ca
ID: 200143300
-------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#define MAXCHAR 21

int T_LEN = 0;
int NUM_RES = 0;
int **MAX;
int **ALLOCATED;
int *AVAIL;

int *get_max_resource_line(char *line);
void read_file(FILE** f, int **max);
void determine_T_LEN(FILE **f);
bool check_safe_state();
void request_(char* buf);
void release(char* buf);
void status();
void release_thread_resources(int thread_num);
void run();
void invoke_command(char *prefix, char *buf);

int *get_max_resource_line(char *line) { 
    int i = 0;
    char *token = strtok(line, ",");
    int *arr = malloc(sizeof(int) * NUM_RES);
    arr[i] = atoi(token);
    while ((token = strtok(NULL, ","))) {i += 1;arr[i] = atoi(token);}
    return arr;
}

void read_file(FILE **f, int **max) { 
    ssize_t read;
    size_t len = 0;
    char *line;
    int *t_arr; 
    int i = 0; 

    while ((read = getline(&line, &len, *f)) != -1) {
        *(MAX + i) = malloc(sizeof(int) * NUM_RES);
        t_arr = get_max_resource_line(line);
        for (int j = 0; j < NUM_RES; j++){MAX[i][j] = t_arr[j];}
        i += 1;
    }

    return;}

void determine_T_LEN(FILE **f) { 
    ssize_t read;
    size_t len = 0;
    char *line;

    while ((read = getline(&line, &len, *f)) != -1) {T_LEN += 1;}
    rewind(*f);
    return;
}

bool check_safe_state() {
    int work[NUM_RES];
    bool finish[T_LEN];
    bool safe = true;
    int i, j, k;

    for (i = 0; i < NUM_RES; i++) {work[i] = AVAIL[i];}

    for (i = 0; i < T_LEN; i++) {finish[i] = false;}

    for (i = 0; i < T_LEN; i++) {
        for (j = 0; j < T_LEN; j++) {
            if (finish[j] == false) {
                for (k = 0; k < NUM_RES; k++) {
                    if (MAX[j][k] - ALLOCATED[j][k] > work[k]) {
                        break;
                    }
                }

                if (k == NUM_RES) {
                    for (k = 0; k < NUM_RES; k++) {
                        work[k] += ALLOCATED[j][k];
                    }

                    finish[j] = true;
                }
            }
        }
    }

    for (i = 0; i < T_LEN; i++) {
        if (finish[i] == false) {
            safe = false;
        }
    }

    return safe;
}

void request_(char *buf) {
    int i;
    bool deny = false;

    char* token = strtok(buf, " ");
    token = strtok(NULL, " ");
    int customer_number = atoi(token);
    int customer_resources[NUM_RES];
    i = 0;

    while ((token = strtok(NULL, " "))) {   
        customer_resources[i] = atoi(token);
        i++;
    }

    for (i = 0; i < NUM_RES; i++) {
        if (customer_resources[i] > MAX[customer_number][i]) {
            deny = true;
        }
    }

    if (deny == false) {
        for (i = 0; i < NUM_RES; i++) {
            if (customer_resources[i] > AVAIL[i]) {
                deny = true;
            }
        }
    }

    if (deny == false) {
        for (i = 0; i < NUM_RES; i++) {
            AVAIL[i] -= customer_resources[i];
            ALLOCATED[customer_number][i] += customer_resources[i];
        }
    }

    if (deny == true || check_safe_state() == false){printf("State is not safe, and request is not satisfied\n");} 
    else {printf("State is safe, and request is satisfied\n");}

    return;
}

void release(char *buf) {
    char* token = strtok(buf, " ");
    int temp[NUM_RES];
    token = strtok(NULL, " "); 
    int thread_num = atoi(token);
    int i = 0;

    while ((token = strtok(NULL, " "))) {   
        temp[i] = atoi(token);
        i++;
    }

    printf("    To release: ");
    for (int j = 0; j < NUM_RES; j++) { 
        AVAIL[j] += temp[j];
        ALLOCATED[thread_num][j] -= temp[j];
        printf("%d ", temp[j]);
    }

    printf("\n");

    printf("    Now Available Resources: ");
    for (int j = 0; j < NUM_RES; j++) { 
        printf("%d ", AVAIL[j]);
    }

    printf("\n");

    printf("    Resources still held by thread: ");
    for (int j = 0; j < NUM_RES; j++) { 
        printf("%d ", ALLOCATED[thread_num][j]);
    }

    printf("\n");
}

void status() {
    printf("Available Resources: \n");
    int i, j; 
    for (i = 0; i < NUM_RES; i++) { 
        printf("%d ", AVAIL[i]);
    } printf("\n");

    printf("Max Resources: \n");
    for (i = 0; i < T_LEN; i++) { 
        for (j = 0; j < NUM_RES; j++) { 
            printf("%d ", MAX[i][j]);
        } printf("\n");
    }

    printf("Allocated Resources: \n");
    for (i = 0; i < T_LEN; i++) {
        for (j = 0; j < NUM_RES; j++) { 
            printf("%d ", ALLOCATED[i][j]);
        } printf("\n");
    }

    printf("Need Resources: \n");
    for (i = 0; i < T_LEN; i++) { 
        for (j = 0; j < NUM_RES; j++) { 
            printf("%d ", MAX[i][j] - ALLOCATED[i][j]);
        } printf("\n");
    }
}

void release_thread_resources(int thread_num) { 
    printf("--> Customer/Thread %d \n", thread_num);
    printf("    Allocated resources: ");
    for (int i = 0; i < NUM_RES; i++) { 
        printf("%d ", ALLOCATED[thread_num][i]);
    } printf(" \n");

    printf("    Needed: ");
    for (int i = 0; i < NUM_RES; i++) {  
        printf("%d ", MAX[thread_num][i] -  ALLOCATED[thread_num][i]);
    } printf(" \n");

    printf("    Available: ");
    for (int i = 0; i < NUM_RES; i++) { 
        printf("%d ", AVAIL[i]);
    } printf(" \n");

    printf("    Thread has started \n");
    printf("    Thread has finished \n");
    printf("    Thread is releasing resources \n");
    for (int i = 0; i < NUM_RES; i++) { 
        AVAIL[i] += ALLOCATED[thread_num][i];
    }

    printf("    New Available: ");
    for (int i = 0; i < NUM_RES; i++) { 
        printf("%d ", AVAIL[i]);
    } printf(" \n");
}

void run() {
    int thread_valid;
    int remaining_threads[T_LEN];
    int remaining_thread_count = T_LEN;
    
    for (int i = 0; i < T_LEN; i++) { 
        remaining_threads[i] = i;
    }
    
    int thread_num;
    int counter = 0;

    printf("Safe Sequence: ");
    for (int i = 0; i < T_LEN; i++) { 
        printf("%d ", remaining_threads[i]);
    }
    
    printf(" \n");
    while (remaining_thread_count > 0 && counter < T_LEN) { 
        for (int i = 0; i < remaining_thread_count; i++) { 
            thread_valid = 1;
            thread_num = remaining_threads[i];

            for (int j = 0; j < NUM_RES; j++) {if (AVAIL[j] < (MAX[thread_num][j] - ALLOCATED[thread_num][j])) {thread_valid = 0;}}

            if (thread_valid == 1) {
                for (; i < remaining_thread_count - 1; i++) {remaining_threads[i] = remaining_threads[i + 1];}
                i++;remaining_thread_count -= 1;release_thread_resources(thread_num);
            }
        } counter++;
    }
}

void invoke_command(char *prefix, char *buf) { 
    if (strcmp(prefix, "Exit") == 0) {
        exit(0);
    } else if (strcmp(prefix, "Run") == 0) { 
        run();
    } else if (strcmp(prefix, "Status") == 0) { 
        status();
    } else if (strcmp(prefix, "RQ") == 0) { 
        request_(buf);
    } else if (strcmp(prefix, "RL") == 0) { 
        release(buf);
    } else { 
        printf("Invalid input, use one of RQ, RL, Status, Run, Exit \n");
    }
}

int main(int argc, char **argv) {
    FILE* f;
    f = fopen("sample_in_banker.txt" , "r");

    AVAIL = malloc(sizeof(int) * NUM_RES);

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {AVAIL[i-1] = atoi(argv[i]);NUM_RES += 1;}
    } else {exit(0);}

    determine_T_LEN(&f);
    MAX = malloc(sizeof(int*) * T_LEN);
    read_file(&f, MAX);
    ALLOCATED = malloc(sizeof(int*) * T_LEN);
    printf("Number of Customers: %d \n", T_LEN);
    printf("Currently Available Resources: ");
    for (int i = 0; i < NUM_RES; i++) {printf("%d ", AVAIL[i]);}
    printf(" \n");

    printf("Maximum resources from file: \n");
    for (int i = 0; i < T_LEN; i++) { 
        ALLOCATED[i] = calloc(NUM_RES, sizeof(int));
        for (int j = 0; j < NUM_RES; j++) {printf("%d ", MAX[i][j]);}
        printf("\n");
    }

    while (1) {
        char buf[MAXCHAR];
        fputs ("Enter Command: ", stdout);   
        while (fgets(buf, MAXCHAR, stdin)) {  
            char *buff = malloc(sizeof(char) * MAXCHAR);
            strcpy(buff, buf);
            char *prefix = strtok(buf, " \n");
            invoke_command(prefix, buff);
            fputs ("Enter Command: ", stdout);
        }
    }
}