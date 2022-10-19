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
struct node *rear = NULL;

void enqueue(int);

void dequeue();


/*
Function Name: getChildren
Input to the method: pid as an integer
Output(Return value): an integer array of the children of the input
Brief description of the task:
Goto the children file of the input pid and return the children that are in the file
*/
int* getChildren(int pid){
    //File Streams for the children file
    FILE *children_file = NULL;
    FILE *children_file2 = NULL;

    //path and pid strings for generating the path to the children file
    char path[100];
    char pid_str[100];
    sprintf(pid_str, "%d", pid);

    //generate the path to the children file
    strcpy(path, "/proc/");
    strcat(path, pid_str);
    strcat(path, "/task/");
    strcat(path, pid_str);
    strcat(path, "/children");

    //read the children file
    children_file = fopen(path, "r");
    children_file2 = fopen(path, "r");
    if (children_file == NULL){ //if the file is empty return NULL
        return NULL;
    }
    else{
        //line buffers for storing the data in the children file
        char line_buff[1000];
        memset(line_buff, 0, sizeof line_buff);
        char line_buff2[1000];
        memset(line_buff2, 0, sizeof line_buff2);

        //get the children from the children file and close the file
        fgets(line_buff, 1000, children_file);
        fgets(line_buff2, 1000, children_file2);
        fclose(children_file);

        char *tokens = strtok(line_buff, " ");//get the children based on the spaces between them
        int i = 1;//starting index of children array
        int length = 0;//starting length of children array
        //get the number of children in the children file
        //which is the length of the children array
        while (tokens != NULL){
            length++;
            tokens = strtok(NULL, " ");
        }

        int* children = calloc(length+1, sizeof(int));//allocate length+1 to the children array for storing ints
        char *tokens2 = strtok(line_buff2, " ");//get the children based on the spaces between them
        children[0] = length;//set the first index of array to the length of the array
        //put the children into the array
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
    //generate the path to the pid
    char path[80];
    sprintf(path, "/proc/%s", pid);
    FILE *file;

    //check if the pid folder exists
    //if it does return 1, if not return 0
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
    printf("Children of %d:", n->pid);//print pid of node n
    //go through the children in node n
    for (int i = 1; i < n->children[0] + 1; i++) {
        char child_pid[100];
        sprintf(child_pid, "%d", n->children[i]);//int to char
        if(pidCheck(child_pid) == 1){//if the childs pid exists put it in the queue
            enqueue(n->children[i]);
        }
        sprintf(child_pid, "%s", "");//reset child_pid variable
        printf(" %d", n->children[i]);//print the children
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
    //check if the number of args is 2
    if(argc > 2 || argc < 2){
        printf("The command takes one argument of a pid.\n");
    }
    else {
        if(pidCheck(argv[1]) == 1) {//check if pid exists
            enqueue(atoi(argv[1]));//put pid and children into queue
            while (front != NULL) {
                display(front);//display the pid and its children
                dequeue();//remove front from queue
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
    nptr->pid = pid;//put the pid into the node nptr
    nptr->children = getChildren(pid);//put the children int array into the node nptr
    nptr->next = NULL;//next is NULL
    //if the rear is NULL set the front and rear to node nptr
    //otherwise rear is the next of nptr
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
Output(Return value): None
Brief description of the task:
move the next node to the front
*/
void dequeue() {
    //if the front is NULL do nothing
    //else make front next node
    if (front == NULL) {
        return;
    } else {
        front = front->next;
    }
}