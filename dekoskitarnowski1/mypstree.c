/*
Author: Michael Dekoski & Evan Tarnowski
Assignment Number: 1
Date of Submission: 10-19-2022
Name of this file: mypstree.c
Short description of contents:
Takes input argument of a pid, gets the children of this pid
and if there are children of the children gets those, until no children remain
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int pid;
    struct node *next;
    int *children;
};

struct node *front = NULL;
struct node *temp = NULL;
struct node *rear = NULL;

void enqueue(int);

struct node *dequeue();


/*
Function Name: getChildren
Input to the method: pid as an integer
Output(Return value): an integer array of the children of the input
Brief description of the task:
Goto the children file of the input pid and return the children that are in the file
*/
int* getChildren(int pid){

    FILE *children_file = NULL;
    FILE *children_file2 = NULL;

    char path[100];
    char pid_str[100];
    sprintf(pid_str, "%d", pid);

    strcpy(path, "/proc/");
    strcat(path, pid_str);
    strcat(path, "/task/");
    strcat(path, pid_str);
    strcat(path, "/children");

    children_file = fopen(path, "r");
    children_file2 = fopen(path, "r");
    if (children_file == NULL){
        return NULL;
    }
    else{
        char line_buff[1000];
        memset(line_buff, 0, sizeof line_buff);
        char line_buff2[1000];
        memset(line_buff2, 0, sizeof line_buff2);
        fgets(line_buff, 1000, children_file);
        fgets(line_buff2, 1000, children_file2);
        fclose(children_file);
        char *tokens = strtok(line_buff, " ");
        int i = 1;
        int length = 0;
        while (tokens != NULL){
            length++;
            tokens = strtok(NULL, " ");
        }
        int* children = calloc(length+1, sizeof(int));
        char *tokens2 = strtok(line_buff2, " ");
        children[0] = length;
        while (tokens2 != NULL){
            children[i] = atoi(tokens2);
            i++;
            tokens2 = strtok(NULL, " ");
        }

        return children;
    }
}


/*
Function Name: pidCheck
Input to the method: character pointer called pid
Output(Return value): Either 1 or 0, 1 if pid exists 0 if pid does not exist
Brief description of the task:
Check whether the pid exists or not
*/
int pidCheck(char* pid){
    char path[80];
    sprintf(path, "/proc/%s", pid);
    FILE *file;

    if (file = fopen(path, "r"))
    {
        fclose(file);
        return 1;
    }
    else
    {
        return 0;
    }
}


/*
Function Name: display
Input to the method: A node pointer
Output(Return value): None
Brief description of the task:
Display the children of the pid from the node
*/
void display(struct node *n){
    int PID_EXISTS = 1;
    int NUM_OF_CHILDREN = 0;
    if (n->children[NUM_OF_CHILDREN] == 0){
        return;
    }
    printf("Children of %d:", n->pid);
    for (int i = 1; i < n->children[NUM_OF_CHILDREN] + 1; i++) {
        char child_pid[100];
        sprintf(child_pid, "%d", n->children[i]);
        if(pidCheck(child_pid) == PID_EXISTS){
            enqueue(n->children[i]);
        }
        sprintf(child_pid, "%s", "");
        printf(" %d", n->children[i]);
    }
    printf("\n");
}


/*
Function Name: main
Input to the method: Number of arguments(argc) and arguments(argv)
Output(Return value): None
Brief description of the task:
Take the pid argument from the command line and use it to find its children
*/
int main(int argc, char *argv[]) {
    int MAX_COUNT = 2;
    int PID_ARG = 1;
    int PID_EXISTS = 1;
    if(argc > MAX_COUNT || argc < MAX_COUNT){
        printf("The command takes one argument of a pid.\n");
    }
    else {
        if(pidCheck(argv[PID_ARG]) == PID_EXISTS) {
            enqueue(atoi(argv[PID_ARG]));
            struct node *n;
            while (front != NULL) {
                n = front;
                display(n);
                dequeue();
            }
        }
        else{
            perror("PID does not exist.");
        }
    }
}


/*
Function Name: enqueue
Input to the method: pid as an integer
Output(Return value): None
Brief description of the task:
put the pid and children into a queue
*/
void enqueue(int pid) {
    struct node *nptr = malloc(sizeof(struct node));
    nptr->pid = pid;
    int* children;
    children = getChildren(pid);
    nptr->children = children;
    nptr->next = NULL;
    if (rear == NULL) {
        front = nptr;
        rear = nptr;
    } else {
        rear->next = nptr;
        rear = rear->next;
    }
}


/*
Function Name: dequeue
Input to the method: None
Output(Return value): current node at the front of the queue
Brief description of the task:
get the current front node, and move the next node to the front
*/
struct node *dequeue() {
    if (front == NULL) {
        return NULL;
    } else {
        temp = front;
        front = front->next;
        return temp;
    }
}