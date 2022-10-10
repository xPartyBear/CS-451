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

int* getChildren(int pid){
    FILE *children_file = NULL;
    FILE *children_file2 = NULL;
    char path[100];
    char *path2;
    char pid_str[100];
    sprintf(pid_str, "%d", pid);
    strcpy(path, "/proc/");
    strcat(path, pid_str);
    strcat(path, "/task/");
    strcat(path, pid_str);
    strcat(path, "/children");
//    if(strcmp(pid, "1") == 0){
//        path2 = "C:\\Users\\Yosgs\\Desktop\\children.txt";
//    }
//    else if(strcmp(pid, "1309") == 0){
//        path2 = "C:\\Users\\Yosgs\\Desktop\\children2.txt";
//    }
//    else{
//        path2 = "C:\\Users\\Yosgs\\Desktop\\children3.txt";
//    }
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

void display(struct node *n){
    if (n->children[0] == 0){
        return;
    }
    printf("Children of %d:", n->pid);
    for (int i = 1; i < n->children[0] + 1; i++) {
        enqueue(n->children[i]);
        printf(" %d", n->children[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if(argc > 2 || argc < 2){
        printf("The command takes one argument of a pid.\n");
    }
    else {
        if(pidCheck(argv[1]) == 1) {
            enqueue(atoi(argv[1]));
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

struct node *dequeue() {
    if (front == NULL) {
        return NULL;
    } else {
        temp = front;
        front = front->next;
        return temp;
    }
}