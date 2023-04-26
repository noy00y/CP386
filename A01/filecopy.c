/*
-------------------------------------------------
Copy contents of one file to destination file
Author: khan3300@mylaurier.ca
ID: 200143300
-------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char *argv[]) {
    int input, output, line;
    char buffer[BUFSIZ];

    // No file input:
    if (argc != 3) {
        fprintf(stdout, "Error: Insufficent Parameters\n");
        exit(1);
    }

    // input file cannot be opened:
    if ((input = open(argv[1], O_RDONLY)) == -1) {
        perror(argv[1]);
        exit(2);
    }

    // output file cannot be opened:
    if ((output = creat(argv[2], 0644)) == -1) {
        perror(argv[2]);
        exit(3);
    }

    // Copy input file line by line to the output file:
    while ((line = read(input, buffer, BUFSIZ)) > 0) {
        if (write(output, buffer, line) != line) {
            perror(argv[2]);
            exit(4);
        } else {
            fprintf(stdout, "Success: File contents have been copied\n");
        }
    }

    // Close files
    close(input);
    close(output);

    return 0;
}