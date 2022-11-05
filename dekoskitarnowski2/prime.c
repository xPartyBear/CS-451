/*
Author: Michael Dekoski & Evan Tarnowski
Assignment Number: 2
Date of Submission: 11-04-2022
Name of this file: prime.c
Short description of contents:
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

const int suspend_signal = 10;
const int continue_signal = 11;
const int terminate_signal = 12;

long unsigned int rand_num;
long unsigned int prime;
int process;
int flag = 0;

/*
Function Name: rand_num_gen
Input to the method: None
Output(Return value): a random number as rand_num
Brief description of the task:
generate a random number for each child to use
*/
long unsigned int rand_num_gen() {
    time_t t;
    long unsigned int rand_max = 9999999999;//max the random number can be
    long unsigned int rand_min = 1000000000;//min the random number can be

    srand((unsigned) time(&t));//set the seed of rand with the system time
    rand_num = rand_min + rand() % (rand_max + 1 - rand_min);//make random number
//    printf("%lu\n", rand_num);
    return rand_num;
}

/*
Function Name: prime_check
Input to the method: prime to check as num
Output(Return value): num(the next prime number)
Brief description of the task:
check for primes starting at num
*/
long unsigned int prime_check(long unsigned int num) {
    bool check = false;
    //while the number is not prime, increment num until the next prime is found
    while(check == false) {
        num++;
        long i;
        long unsigned int m;
        check = true;
        m = num / 2;
        for(i = 2; i <= m; i++) {
            if (num % i == 0) {//if num is divisible by i, it is not prime
                //printf("Entered number is not prime");
                check = false;
                break;
            }
        }
    }
    //1 not prime
    //0 is prime
    return num;
}

/*
Function Name: start
Input to the method: None
Output(Return value): None
Brief description of the task:
print the start message and check primes
*/
void start(){
    //print start message
    printf("Process %d: My PID is %d: I just got started.\n"
           "I am starting with the number %lu to find the next prime number.\n\n", process, getpid(), rand_num);
    fflush(stdout);
    prime = prime_check(rand_num);//check if the random number is prime
    while(flag == 0){
        prime = prime_check(prime);//check all primes after that
    }
}

/*
Function Name: resume
Input to the method: None
Output(Return value): None
Brief description of the task:
print the resumed message and check primes
*/
void resume(){
    //print resumed message
    printf("Process %d: My PID is %d: I just got resumed.\n\n", process, getpid());
    fflush(stdout);
    while(flag == 0) {//while flag is 0(process is resumed)
        prime = prime_check(prime);//check primes
    }
}

/*
Function Name: suspend
Input to the method: None
Output(Return value): None
Brief description of the task:
print the suspension message
*/
void suspend(){
    //print suspension message
    printf("Process %d: my PID is %d: I am about to be suspended...\n"
           "Highest prime number I found is %lu.\n\n",process, getpid(), prime);
    fflush(stdout);
}

/*
Function Name: terminate
Input to the method: None
Output(Return value): None
Brief description of the task:
print the completion message
*/
void terminate(){
    //print completion message
    printf("Process %d: my PID is %d: I completed my task and I am exiting.\n"
           "Highest prime number I found is %lu.\n\n",process, getpid(), prime);
    fflush(stdout);
}

/*
Function Name: child_handler
Input to the method: signal as signum
Output(Return value): None
Brief description of the task:
The handler for receiving signals from the parent
*/
void child_handler(int signum){
    if(signum == suspend_signal){//if suspend signal
        flag = 1;
        suspend();
    }
    else if(signum == continue_signal){//if continue signal
        flag = 0;
        resume();
    }
    else if(signum == terminate_signal) {//if terminate signal
        flag = -1;
        terminate();
    }
}

/*
Function Name: main
Input to the method: Number of arguments(argc) and arguments(argv)
Output(Return value): 0
Brief description of the task:
init the signals, stay until terminated
*/
int main(int argc, char* argv[]){

    //set process to the process from input.txt
    process = atoi(argv[1]);

    //init the signals for suspending, resuming, and terminating
    signal(continue_signal, child_handler);
    signal(suspend_signal, child_handler);
    signal(terminate_signal, child_handler);

    //generate random number
    rand_num = rand_num_gen();

    //run the starting function
    start();

    //stay until terminated
    while(1){
        //if terminated(flag=-1) return 0
        if(flag == -1){
            return 0;
        }
    }
}