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
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

int* id_burst;
struct sigaction sa;
struct itimerval timer;

int sigStart = 10;
int sigSus = 11;
int sigExit = 12;

int RoundRobin(int id){
    printf("Start Round Robin\n");
    int pid;
    char id_str[10];
    if(id >= 0 && id <= 10){
        if ((pid = fork()) < 0) {
            printf("Fork Failed\n");
        } else if (pid == 0) { //child
            sprintf(id_str, "%d", id);
            execl("./prime", id_str, NULL);
        }
    }
    if(pid > 0){
        return pid;
    }
}

int inCounter = 0;
int i = 0;
int readyQueue[100];
int first = 0;
int currProc[10];
int nextProcIndex = 0;

void timer_handler(int signum) {
    printf("Start Handler\n");
    static int clock = 0;
    int printed = 0;

    if((i < inCounter) && (id_burst[i+1] <= clock)) {
        printf("Clock: %d\n", clock);
        printf("inCounter: %d\n", inCounter);
        printf("id_burst: %d\n", id_burst[i+1]);
        readyQueue[i] = id_burst[i];
        readyQueue[i + 1] = id_burst[i + 3];
        readyQueue[i + 2] = id_burst[i + 6];
        readyQueue[i + 3] = id_burst[i + 9];
        if(clock == 1 && printed == 0){
            printf("Scheduler: Time Now: %d second\n", clock);
            printed = 1;
        }
        else if(printed == 0) {
            printf("Scheduler: Time Now: %d seconds\n", clock);
            printed = 1;
        }
    }
        //start processing the readyQueue
        //if already processing
    if(readyQueue[0] > -1){
        //index
        int j = 0;
        int terminated = 0;
        //decrease bt of current process
        currProc[2] = currProc[2] - 1;

        //-------------------------------TERMINATE-------------------------
        //if if BT is 0 end it by setting variables out of bounds
        if(currProc[2] == 0){
            if(clock == 1 && printed == 0){
                printf("Scheduler: Time Now: %d second\n", clock);
                printed = 1;
            }
            else if(printed == 0) {
                printf("Scheduler: Time Now: %d seconds\n", clock);
                printed = 1;
            }
            printf("Terminating Process %d\n", currProc[0]);
            terminated = 1;
            kill(currProc[3], sigExit);
            //readyQueue[(currProc[0] * 3)] = 99;
            readyQueue[(currProc[0] * 4 + 1)] = 99;
            readyQueue[(currProc[0] * 4 + 2)] = 99;
            readyQueue[(currProc[0] * 4 + 3)] = 99;
            currProc[1] = 99;
            currProc[2] = 99;
            currProc[3] = 99;
        }

        int processedBT = currProc[2];
        int lowestbt = 0;
        int currIndex = 0;
        currIndex = currProc[0] * 4;
        readyQueue[currIndex + 2] = processedBT;

        //compare current process finished or not to rest of readyQueue
        //will reach -1 when finished scanning ready queue
        //------------------------------PROCESS-------------------------
        while(readyQueue[j] >= 0) {
            //if ready process BT is less than current process BT
            //find context switch is needed
            if ((readyQueue[j + 2] < currProc[2] || (readyQueue[j + 2] == currProc[2] && readyQueue[j] < currProc[0]))&& readyQueue[j + 2] != 99) { // && readyQueue[j + 2] != 99
                //printf("p0 bt: %d\n", readyQueue[2]);
                lowestbt = readyQueue[j + 2];
                nextProcIndex = (readyQueue[j] * 4);
            }
            //printf("nextProcIndex: %d\n", nextProcIndex);
            j = j + 4;

        }

        //IF CONTEXT SWITCHING
        if(currProc[0] != readyQueue[nextProcIndex]) {
            //main context switch

            if(printed == 0) {
                printf("Scheduler: Time Now: %d seconds\n", clock);
                printed = 1;
            }

            //first time switching to process
            if(readyQueue[nextProcIndex + 3] == -1){
                if(terminated != 1) {
                    printf("Suspending Process %d with remaining time of %d seconds and\n", currProc[0], currProc[2]);
                }
                currProc[0] = readyQueue[nextProcIndex];
                printf("Scheduling Process %d\n", currProc[0]);
                currProc[1] = readyQueue[nextProcIndex + 1];
                currProc[2] = readyQueue[nextProcIndex + 2];
                kill(currProc[3], sigSus);
                readyQueue[nextProcIndex + 3] = RoundRobin(currProc[0]);
                currProc[3] = readyQueue[nextProcIndex + 3];
                //no touch give time for process to start
                usleep(300000);
                kill(currProc[3], sigStart);
            }
                //resuming process
            else {
                if(terminated != 1) {
                    printf("Suspending Process %d with remaining time of %d seconds and\n", currProc[0], currProc[2]);
                }
                currProc[0] = readyQueue[nextProcIndex];
                currProc[1] = readyQueue[nextProcIndex + 1];
                currProc[2] = readyQueue[nextProcIndex + 2];
                printf("Resuming Process %d whose remaing time is %d seconds\n", currProc[0], currProc[2]);
                //kill the old process
                kill(currProc[3], sigSus);
                currProc[3] = readyQueue[nextProcIndex + 3];
                //start new process
                kill(currProc[3], sigStart);
            }
        }
    }
    //increment clock
    clock++;
}

int main(int argc, char *argv[]){
    printf("Start main\n");
    FILE *infile = NULL;
    char line_buff[1000];
    memset(line_buff, 0, sizeof line_buff);
    if(argc > 3 || argc < 3){
        perror("The command takes 2 arguments of a file and number");
    }
    else {
        infile = fopen(argv[1], "r");
        fgets(line_buff, 1000, infile);
        while(fgets(line_buff, 1000, infile) != NULL){
            id_burst = calloc(1000, sizeof(int));
            int i = 0;
            char *tokens = strtok(line_buff, " ");
            while (tokens != NULL){
                printf("%s\n", tokens);
                id_burst[i] = atoi(tokens);
                i++;
                tokens = strtok(NULL, " ");
            }
        }
    }
    fclose(infile);
    /* Install timer_handler as the signal handler for SIGALRM. */
    memset(&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGALRM, &sa, NULL);

/* Install timer_handler as the signal handler for SIGALRM. */
/* The timer goes off 10 seconds after installation of the timer.*/
    timer.it_value.tv_sec = atoi(argv[2]);
    timer.it_value.tv_usec =0;
/* ... and every 10 seconds after that. */
    timer.it_interval.tv_sec = atoi(argv[2]);
    timer.it_interval.tv_usec =0;
/* Start a real timer. It counts down whenever this process is
executing. */
    setitimer(ITIMER_REAL, &timer, NULL);
    while(1) { }
}