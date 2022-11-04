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

int RoundRobin(int id){
//    printf("Start Round Robin\n");
    int pid;
    char id_str[10];
    if(id >= 0 && id <= 10){
        if ((pid = fork()) < 0) {
            printf("Fork Failed\n");
            fflush(stdout);
        } else if (pid == 0) { //child
            sprintf(id_str, "%d", id);
            char *args[] = {"./prime", id_str, NULL};
            execvp("./prime", args);
        }
    }
    if(pid > 0){
        return pid;
    }
}

int handler_index = 0;
int bt_time;
int clock_timer = 0;
int length = 0;

void timer_handler(int signum){
    printf("Start Handler: %d\n", signum);
    if((id_burst[handler_index+1] != 0) && (id_burst[handler_index+3] == SIGCONT)){
        printf("Suspending: %d\n\n", id_burst[handler_index]);
        fflush(stdout);
        kill(id_burst[handler_index+2], SIGSTOP);
        id_burst[handler_index+3] = SIGSTOP;
        id_burst[handler_index+1] -= bt_time;
//        clock_timer+=bt_time;
        handler_index+=4;
    }
    else if(id_burst[handler_index+1] == 0){
        fflush(stdout);
        kill(id_burst[handler_index+2], SIGTERM);
        id_burst[handler_index+3] = SIGTERM;
    }

    printf("Scheduler: Time Now: %d seconds\n", clock_timer);
    fflush(stdout);
    if(((length*2)) <= handler_index){
        handler_index = 0;
    }

    if(id_burst[handler_index+3] == SIGSTOP){
        fflush(stdout);
        kill(id_burst[handler_index+2], SIGCONT);
        id_burst[handler_index+3] = SIGCONT;
    }
    else if(id_burst[handler_index+3] == 0){
        id_burst[handler_index+2] = RoundRobin(id_burst[handler_index]);
        printf("Scheduling to Process %d (Pid %d) for the time slice of %d seconds.\n\n"
                ,id_burst[handler_index], id_burst[handler_index+2], bt_time);
        fflush(stdout);
        kill(id_burst[handler_index+2], SIGCONT);
        id_burst[handler_index+3] = SIGCONT;
    }

    clock_timer+=bt_time;
//    handler_index+=4;
}

int main(int argc, char *argv[]){
    printf("Start main\n");

    bt_time = atoi(argv[2]);

    FILE *infile = NULL;
    FILE *infile2 = NULL;
    char line_buff[100];
    char line_buff2[100];
    if(argc > 3 || argc < 3){
        perror("The command takes 2 arguments of a file and number\n\n");
        fflush(stdout);
    }
    else {
        infile2 = fopen(argv[1], "r");
        fgets(line_buff2, 100, infile2);

        while (fgets(line_buff2, 100, infile2) != NULL) {
            char *tokens2 = strtok(line_buff2, " ");
            while (tokens2 != NULL) {
                length++;
                tokens2 = strtok(NULL, " ");
            }
        }
        id_burst = calloc(length*2, sizeof(int));
        int i = 0;
        int j = 0;
        infile = fopen(argv[1], "r");
        fgets(line_buff, 100, infile);
        while(fgets(line_buff, 100, infile) != NULL){
            char *tokens = strtok(line_buff, " ");
            while (tokens != NULL){
//                printf("%s\n", tokens);
                id_burst[i+j] = atoi(tokens);
                j++;
                tokens = strtok(NULL, " ");
            }
            i+=2;
        }
        fclose(infile);
    }

//    for(int i = 0; i <= 24; i++){
//        printf("id_burst[%d]: %d\n", i, id_burst[i]);
//    }
/* Install timer_handler as the signal handler for SIGALRM. */
    memset(&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGALRM, &sa, NULL);

/* Install timer_handler as the signal handler for SIGALRM. */
/* The timer goes off 10 seconds after installation of the timer.*/
    timer.it_value.tv_sec = 3;
    timer.it_value.tv_usec =0;
/* ... and every 10 seconds after that. */
    timer.it_interval.tv_sec = bt_time;
    timer.it_interval.tv_usec =0;
/* Start a real timer. It counts down whenever this process is
executing. */
    setitimer(ITIMER_REAL, &timer, NULL);
    while(1) { }


}