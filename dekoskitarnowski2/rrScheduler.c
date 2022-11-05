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

int handler_index = 0;
int bt_time;
int clock_timer = 0;
int length = 0;
int completed_count = 0;

const int suspend_signal = 10;
const int continue_signal = 11;
const int terminate_signal = 12;

/*
Function Name: RoundRobin
Input to the method: process id as an integer
Output(Return value): the pid of the child
Brief description of the task:
Fork a child and return the pid
*/
int RoundRobin(int id){
    int pid;
    char id_str[10];
    if(id >= 0 && id <= 10){
        //if the fork failed print error
        if ((pid = fork()) < 0) {
            printf("Fork Failed\n");
            fflush(stdout);
        } else if (pid == 0) { //child
            sprintf(id_str, "%d", id);
            char* args[] = {"./prime", id_str, NULL};
            //run the prime function as the child
            execvp("./prime", args);
        }
    }
    //return the pid of the child`
    if(pid > 0){
        return pid;
    }
}

/*
Function Name: timer_handler
Input to the method: signal as signum
Output(Return value): None
Brief description of the task:
The handler for sending signals to the child processes
*/
void timer_handler(int signum){
    printf("Scheduler: Time Now: %d seconds\n", clock_timer);//print current processing time
    fflush(stdout);
    //if there is time left for the process and it is running then suspend it
    if((id_burst[handler_index+1] != 0) && (id_burst[handler_index+3] == continue_signal)){
        id_burst[handler_index+3] = suspend_signal;//set the current signal to SIGSTOP
        id_burst[handler_index+1] -= bt_time;//remove time quantum from the burst time

        //if there is no more time left for the process and it is suspended then terminate it
        if((id_burst[handler_index+1] == 0) && (id_burst[handler_index+3] == suspend_signal)){
            id_burst[handler_index+1] = -1;//set the burst time to -1
            id_burst[handler_index+3] = terminate_signal;//set the current signal to SIGTERM
            kill(id_burst[handler_index+2], terminate_signal);//signal the child to terminate
            id_burst[handler_index+2] = -1;//set the child pid to -1
            completed_count++;//increment completed_count
        }
        else{
            kill(id_burst[handler_index+2], suspend_signal);//signal the child to suspend
        }
        handler_index+=4;
    }
    else if(id_burst[handler_index+3] == terminate_signal){
        handler_index+=4;
    }

    //when the index is more that the length of id_burst reset it to 0
    if((length*2) <= handler_index){
        handler_index = 0;
    }

    //if the process is suspended, resume it
    if((id_burst[handler_index+3] == suspend_signal) && (id_burst[handler_index+1] > 0)){
        id_burst[handler_index+3] = continue_signal;//set the current signal to SIGCONT
        kill(id_burst[handler_index+2], continue_signal);//signal the child to resume
    }
    //if the process has not been forked yet(No value in pid(handler_index+2) or current signal(handler_index+3))
    if((id_burst[handler_index+3] == 0) || (id_burst[handler_index+2] == 0)){
        id_burst[handler_index+2] = RoundRobin(id_burst[handler_index]);//set the pid
        printf("Scheduling to Process %d (Pid %d) for the time slice of %d seconds.\n\n"
               ,id_burst[handler_index], id_burst[handler_index+2], bt_time);//print the scheduling of the process
        fflush(stdout);
        id_burst[handler_index+3] = continue_signal;//set the current signal to SIGCONT
    }

    clock_timer+=bt_time;//add time quantum to the clock
}

/*
Function Name: main
Input to the method: Number of arguments(argc) and arguments(argv)
Output(Return value): 0
Brief description of the task:
Take the file and time quantum arguments to use in the handler
Run a timer that will call the handler every time quantum
*/
int main(int argc, char *argv[]){
//    printf("Start main\n");
    //change the time quantum to an int
    bt_time = atoi(argv[2]);

    //set up file pointers and line buffers
    FILE *infile = NULL;
    FILE *infile2 = NULL;
    char line_buff[100];
    char line_buff2[100];

    //when then arguments are more or less than 3, print error
    if(argc > 3 || argc < 3){
        printf("The command takes 2 arguments of a file and number\n");
        fflush(stdout);
        exit(1);
    }
    else {
        //use the file pointer to get the length for id_burst array
        infile2 = fopen(argv[1], "r");
        fgets(line_buff2, 100, infile2);
        while (fgets(line_buff2, 100, infile2) != NULL) {
            char *tokens2 = strtok(line_buff2, " ");
            while (tokens2 != NULL) {
                length++;
                tokens2 = strtok(NULL, " ");
            }
        }

        //use file pointer to populate id_burst array with a length of double the input
        id_burst = calloc(length*2, sizeof(int));
        int i = 0;
        int j = 0;
        infile = fopen(argv[1], "r");
        fgets(line_buff, 100, infile);
        while(fgets(line_buff, 100, infile) != NULL){
            char *tokens = strtok(line_buff, " ");
            while (tokens != NULL){
                id_burst[j+i] = atoi(tokens);
                j++;
                tokens = strtok(NULL, " ");
            }
            i+=2;
        }
        fclose(infile);
        fclose(infile2);
    }

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

    while(1) {
        if(completed_count == (length/2)) {//when all processed are completed exit
            exit(0);
            return 0;
        }
    }
}