/*
-------------------------------------------------
Calculates Assignment Average
Author: khan3300@mylaurier.ca
ID: 200143300
-------------------------------------------------
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COLS 10
#define MAX_ROWS 10

int main(int arg1, char *arg2[]) {
    
	//Initialize required variables
	char *row_num; char *filename;

	char buf[25];
	int grades[MAX_COLS][MAX_ROWS];
	int cols = 0;

	int total_rows = 0; // also used in iteration
	int total_cols = 0; // also used in iteration
	

	if (arg1>2 || arg1<2) {printf("Error: Incorrect number of arguments given, please try again.\n");exit(1);} // check if wrong number of inputs were given
	filename = arg2[1]; // If the correct number of arguments were given, set filename

	// grade reading component
	FILE *fp = fopen(filename, "r"); // open file to read for grades
	int grade; 
	int num = 0;
	while ((row_num = fgets(buf, sizeof(buf), fp)) != NULL) { // get values of a student into the buffer
		while (sscanf(row_num, "%d%n", &grade, &num) == 1) { 
			grades[total_rows][cols] = grade;  
			if (total_rows == 0){total_cols++;}
			row_num += num; // increase row_num relative to num
			cols++; // increase number of cols
		}
		total_rows++; // increment total number of rows
		cols = 0; // reset col number for new student
	}

    // close file after reading grades
	fclose(fp);

	
	for (int i = 0; i < total_rows; i++) { //iteration
		int gradTA = fork();
		if (gradTA < 0) {
			printf("Error: Failed to create GradTA process\n");exit(1);
		} else if (gradTA == 0) {
		
			for (int a = 0; a < total_cols; a++) { //create the required TA processes 

				int TA = fork(); // create TA process
				if (TA < 0) {printf("Error: Failed to create TA process\n"); exit(1);}

				else if (TA == 0) { // TA Process
					
					// get sum of grades
					int sum = 0;
					for (int j = 0; j < total_rows; j++) {
						sum += grades[j][a];
					}

					// calculate the average 
					double average = (double) sum / total_rows;
					printf("Assignment %d - Average = %f \n", a + 1,average);
					average = 0;
					break;

				} else if (TA > 0) {wait(NULL);} // Grad TA Process

			} break;

		} else if (gradTA > 0) {wait(NULL);break;} // Teacher Process

	} return 0;
}