#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

void handler(int signum);
void resume();
void suspend();
void terminate();

long unsigned int rand_num;
long unsigned int prime;
int boolean = 0;

long unsigned int rand_num_gen() {
    time_t t;
    long unsigned int rand_max = 9999999999;
    long unsigned int rand_min = 1000000000;

    srand((unsigned) time(&t));
    rand_num = rand_min + rand() % (rand_max + 1 - rand_min);
    printf("%lu\n", rand_num);
    return rand_num;
}

long unsigned int prime_check(long unsigned int num) {
    bool check = false;
    while(check == false) {
        num++;
        long i;
        long unsigned int m;
        check = true;
        m = num / 2;
        for(i = 2; i <= m; i++) {
            if (num % i == 0) {
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

int main() {
    //Start handlers
    signal(sigStart, handler);
    signal(sigSus, handler);
    signal(sigExit, handler);

    //long unsigned int rand_num = rand_num_gen();
    rand_num = rand_num_gen();
    //prime = prime_check(prime);

    return 0;
}

void handler(int signum){
// Write handler code here.

    if (signum == sigStart) {
        boolean = 1;
        resume();
    }
    if (signum == sigSus) {
        boolean = 0;
        //resume();
//            sleep(.4);
        suspend();
    }
    if (signum == sigExit) {
        //sleep(.3);
        terminate();
    }
}

void resume(){
    //start
    if (boolean == 1) {
        printf("Process %d: My PID is %d: I just got started. \n"
               "I am starting with the number %llu to find the next prime number.\n", process, getpid(), test);
    }

    while(true){
        prime = prime_check(prime);
    }
}
void suspend(){
    printf("Process %d: my PID is %d: I am about to be suspended... Highest\n"
           "prime number I found is %llu.\n",process, getpid(), prime);

}
void terminate(){
    printf("Process %d: my PID is %d: I completed my task and I am exiting.\n"
           "Highest prime number I found is %llu.\n",process, getpid(), prime);
    boolean = 2;
}
