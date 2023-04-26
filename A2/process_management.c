/*
-------------------------------------------------
Author: khan3300@mylaurier.ca
ID: 200143300
-------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>


void writeOuput (char *command, char *output) { 
    FILE *fp;
    fp = fopen("output.txt", "a");
	fprintf(fp, "The output of: %s : is\n", command);
	fprintf(fp, ">>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<\n", output);	
    fclose(fp);
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {printf("Error: Invalid number of arguments provided\n");return 1;} // Check for invalid number of arguments
    
    // declare variables required for reading and storing grades
    int fd, shmfd; char *shmptr;FILE *fp;
    // Attempt to open file (check if file is openable)
    if ((fd = open(argv[1], O_RDONLY)) == -1) {printf("Error: Failed to open file for reading\n");exit(1);}   
    
    // Create shared memory for storing grades
    shmfd = shm_open("sh_mem", O_RDWR | O_CREAT, 0666);
    ftruncate(shmfd, 4096);

    char **commands; // Declare dynamic array for reading commands
    fp = fopen(argv[1], "r"); 
    commands = (char**)malloc(4096 * sizeof(char*));
    for (int i = 0; i < 4096; i++) { // allocate required memory for dynamic array 
        commands[i] = (char*)malloc(4096 * sizeof(char));
    }

    // load/read commands onto the dynamic array
    int i = 0; 
    while (fgets(commands[i], 4096, fp) != NULL && i < 4096) {i++;}


    // close file after reading commands
    fclose(fp);

    i = 0;
    while(commands[i][0]) { 
        char command[4096];
        strcpy(command, commands[i]); // copy current command
        int pid = fork(); // fork to create child process responsible for handling command
        
        if (pid == -1) {
            printf("Error: Failed to fork from %i.\n", getpid());exit(1);
        } else if (pid == 0) { // child process
            char *cmd = strtok(command, " "); // break down string
            char *argv[5];
            argv[0] = cmd;
            
            int i = 1;
            while (1) {
                char *arg = strtok(NULL, " ");
                if (!arg) break; // if we filed to delim the string
                argv[i++] = arg;
            }
            argv[i] = NULL; 

            for (int i = 0; i < 5 && argv[i]; i++) {
                int len = strlen(argv[i]);
                if (argv[i][len - 1] == '\n') {argv[i][len - 1] = '\0';}
            }

            // producer component + handling fd and command execution
            int fd = shm_open("sh_mem", O_CREAT | O_RDWR, 0666);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            if (execvp(argv[0], argv) == -1) {printf("Error: Unable to execute command given to %i\n",getpid());exit(1);}
            
        } else if (pid > 0) { // parent process
            waitpid(pid, NULL, 0);
            char buf[4096]; // declare buffer with size for parent process
            shmptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0); // read from share memory
            sprintf(buf, "%s", shmptr); // to shared memory
            printf("buffer: %s \n", buf); 
            writeOuput(command, buf); // invoke function to write command execution results to the output file
            memset(shmptr, 0, 4096); 
            munmap(shmptr, 4096); // remove addressability
        }
        i++;
    }

    close(shmfd); // get rid of share memory
    return 0;
}

