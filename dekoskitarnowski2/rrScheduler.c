/*
Author: Michael Dekoski & Evan Tarnowski
Assignment Number: 2
Date of Submission: 11-04-2022
Name of this file: rrScheduler.c
Short description of contents:
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE *infile = NULL;
    char line_buff[1000];
    memset(line_buff, 0, sizeof line_buff);
    if (argc > 3 || argc < 3){
        perror("The command takes 2 arguments of a file and number");
    }
    else {
        infile = fopen(argv[1], "r");
        fgets(line_buff, 1000, infile);
        while(fgets(line_buff, 1000, infile) != NULL){
            int* id_burst = calloc(1000, sizeof(int));//allocate length+1 to the children array for storing ints
            int i = 0;
            char *tokens = strtok(line_buff, " ");//get the children based on the spaces between them
            //put the children into the array
            while (tokens != NULL){
                printf("%s", tokens);
                id_burst[i] = atoi(tokens);
                i++;
                tokens = strtok(NULL, " ");
            }
        }
    }
}