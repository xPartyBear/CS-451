/*
Author: Michael Dekoski & Evan Tarnowski
Assignment Number: 3
Date of Submission: 11-23-2022
Name of this file: makingmusic.c
Short description of contents:
 simulate a set of threads that make music and synchronize them using semaphores.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t mutex;
pthread_t thread_id[2];

void thread_task(int);

int main(int argc, char* argv[]){

    sem_init(&mutex, 0, 1);

    if(strcmp(argv[1], "-nodelay") == 1){
        if(argc < 5 || argc > 5){
            printf("-nodelay takes 3 arguments, vocalists, composers, and sound proof rooms\n");
            exit(1);
        }
        else{
            for (int i =0; i <2; i++)
                pthread_create(&thread_id[i], NULL, thread_task, (void *) i);
            for (int i =0; i <2; i++)
                pthread_join(thread_id[i], NULL);
        }
    }
    else if(strcmp(argv[1], "-randomdelay") == 1){
        if(argc < 7 || argc > 7){
            printf("-randomdelay takes 5 arguments, vocalists, composers, sound proof rooms, maxWanderTime, and maxSoundRoomUsageTime\n");
            exit(1);
        }
        else{
            for (int i =0; i <2; i++)
                pthread_create(&thread_id[i], NULL, thread_task, (void *) i);
            for (int i =0; i <2; i++)
                pthread_join(thread_id[i], NULL);
        }
    }
}
void thread_task(int i) {
    //Code for the thread task goes here
    sem_wait(&mutex);
    printf("Entered thread: %d\n", i);

    //critical section

    printf("Exiting thread: %d\n", i);
    sem_post(&mutex);
    pthread_exit(0); // this code returns to the corresponding pthread_join issued in main()
}