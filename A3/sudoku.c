/*
-------------------------------------------------
Validates a Sudoku Problem for Validity
Author: khan3300@mylaurier.ca
ID: 200143300
-------------------------------------------------
*/

// Libraries:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>

// Structures:
struct sudoku_container {
    int sudoku[9][9];
    int conditions[3];
};

// General Functions:
void read_sudoku(int sudoku[9][9], FILE** file) { 
    int i, j; 
    for(i = 0; i < 9; i++) { 
        for(j = 0; j < 9; j++) {
            char s[] = "0";
            fscanf(*file, " %c", &s[0]);
            sudoku[i][j] = atoi(s);
        }
    }
    return;
}

void print_sudoku(int sudoku[9][9]) { 
    printf("Sudoku Puzzle Solution is:\n");
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
        fflush(stdout);
    }
    return;
}

bool check_rows_aux(int sudoku[9][9]) {
    int i, j;
    int count;
    for (i = 0; i < 9; i++) {
        count = 0;
        for (j = 0; j <= 8; j++) {
            count += pow(10, sudoku[i][j]);
        }
        if (count != 1111111110) { 
            return false;
        }
    }
    return true;
}

bool check_cols_aux(int sudoku[9][9]) { 
    int i, j;
    int count;
    for (j = 0; j < 9; j++) {
        count = 0;
        for (i = 0; i < 9; i++) {
            count += pow(10, sudoku[i][j]);
        }
        if (count != 1111111110) { 
            return false;
        }
    }
    return true;
}

bool check_box(int sudoku[9][9], int rowStart, int columnStart) { 
    int count = 0;
    int i, j; 
    for (i = rowStart; i < rowStart+3; i++) {
        for (j = columnStart; j<columnStart + 3; j++) { 
            count += pow(10, sudoku[i][j]);
        }
    }
    if (count != 1111111110)
        return false;
    return true;
}

bool check_boxes_aux(int sudoku[9][9]) { 
    int i;
    int count; 
    for (i = 0; i < 9; i+= 3) { // decides starting row of box
        if (!check_box(sudoku, i, 0)) return false;
        if (!check_box(sudoku, i, 3)) return false;
        if (!check_box(sudoku, i, 6)) return false;
    }
    return true;
}

// Parent Thread Functions:
void *check_rows(void *s_container) {
    struct sudoku_container *container = (struct sudoku_container*)s_container;
    if (check_rows_aux((*container).sudoku)) {
        (*container).conditions[1] = 1;
    }
}
void *check_cols(void *s_container) {
    struct sudoku_container *container = (struct sudoku_container*)s_container;
    if (check_cols_aux((*container).sudoku)) {
        (*container).conditions[1] = 1;
    }
}
void *check_boxes(void *s_container) {
    struct sudoku_container *container = (struct sudoku_container*)s_container;
    if (check_boxes_aux((*container).sudoku)) {
        (*container).conditions[1] = 1;
    }
}

int main(int argc, char *argv[]) {
    // Declarations:
    int sudoku[9][9];

    // Creating File
    FILE* file;
    if (argc == 1) {file = fopen("sample_in_sudoku.txt" , "r");
    } else { file = fopen(argv[1] , "r");}

    // Reading from file and printing sudoku matrix
    read_sudoku(sudoku, &file);
    print_sudoku(sudoku);    

    // Storing sudoku matrix and validation condition set in structure:
    int conditions[3];
    struct sudoku_container s_container;
    memcpy(&s_container.sudoku, &sudoku, 81*sizeof(int)); 
    memcpy(&s_container.conditions, &conditions, 3 * sizeof(int));
    
    // Creating Parent Threads and Passing Struct:
    pthread_t id[1];
    pthread_create(&id[0], NULL, check_rows, (void*)&s_container);
    pthread_create(&id[0], NULL, check_cols, (void*)&s_container);
    pthread_create(&id[0], NULL, check_boxes, (void*)&s_container);   

    // Joining Threads:
    for (int i = 0; i < 3; i++) {
        pthread_join(id[i], NULL);
    }

    // Waiting for Thread Termination:
    for (int i = 0; i < 1; i++) {
        if (s_container.conditions[i] != 1) {
            printf("Sudoku Puzzle is Not Valid\n");
            return 1;
        }
    }
    printf("Sudoku Puzzle is Valid");
    return 0;
}