/*
-------------------------------------------------
Perform Operations on Working Directory
Author: khan3300@mylaurier.ca
ID: 200143300
-------------------------------------------------
*/

// Libaries
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>

/*
    Creates subdirectory under current directory
*/
void create_directory() {
    // Create directory
    printf("Enter the Directory name you want to create: ");
    int directory;
    char directory_name[256]; 
    scanf("%s", directory_name); // get input
    directory = mkdir(directory_name, 0777);

    // Check if Created:
    if (!directory)
        printf("Directory Created\n");
    else {
        perror("Error: directory not created\n");
    }
    return;
}

/*
    Removes directory
*/
void remove_directory() {
    printf("Enter the Directory name you want to remove: ");
    char directory_name[249];
    char cmd[256] = { 0 };
    scanf("%s", directory_name);
    int res = 0;
    sprintf(cmd, "rm -rf %s", directory_name);
    res = system(cmd);
    if (res == 0)
        printf("Directory Deleted\n");
    else {
        perror("Error: directory not deleted\n");
    }
    return;
}

/*
    Get current directory
*/
void get_directory() {
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd))!= NULL) {
        printf("Current Working Directory is: %s\n", cwd);
    } else {
        perror("Error: getcwd()\n");
    }
    return;
}

/*
Changing a directory (to one above)
*/
void set_directory() {
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Working Directory Before Operation: %s\n", getcwd(cwd, 100));
        chdir("..");
        printf("Working Directory After Operation: %s\n", getcwd(cwd, 100));
    }
    else {perror("Error: getcwd()\n");}
    return;
}

/*
Reading a directory
*/
void read_directory() {
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if (opendir(cwd) != NULL) {
            DIR *directory = opendir(cwd);
            struct dirent *sub_directory;
            while ((sub_directory = readdir(directory)) != NULL) {
                printf("%s\n", sub_directory->d_name);
            }
        } else {
            perror("Error: opendir()\n");
        }
    } else {
        perror("Error: getcwd()\n");
    }
    return;
}

/*
Close file directory
*/
void close_directory() {
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if (opendir(cwd) != NULL) {
            DIR *directory = opendir(cwd);
            closedir(directory);
            printf("Directory Closed Succesfully\n");            
        } else {
            perror("Error: opendir()\n");
        }
    } else {
        perror("Error: getcwd()\n");
    }    
    return;
}

// Program Driver
int main() 
{
    const char interface[10][60] = {
        {"Select the option(s) appropriately by entering the number:\n"},
        {"\tEnter 1 for creating a directory\n"},
        {"\tEnter 2 for removing directory\n"},
        {"\tEnter 3 for printing working directory\n"},
        {"\tEnter 4 for changing directory one level up\n"},
        {"\tEnter 5 for reading the contents of directory\n"},
        {"\tEnter 6 for closing the directory\n"},
        {"\tEnter q to exit the program\n"}
    };
    char option;
    while (option != 'q') {
        fflush(stdout);
        for (int i = 0; i < 10; i++) {
            printf("%s", interface[i]);
        }
        scanf(" %c", &option);

        // Call Various Directory Methods:
        if (option == '1') {create_directory();}
        else if (option == '2') {remove_directory();}
        else if (option == '3') {get_directory();}
        else if (option == '4') {set_directory();}
        else if (option == '5') {read_directory();}
        else if (option == '6') {close_directory();}
    }
    return 0;
}