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

int vocalists;
int composers;
int soundproof;
int randRoomUsage;
int randWander;
int flag = 0;

void vocalists_task(int);
void composers_task(int);

/*
Function Name: main
Input to the method: Number of arguments(argc) and arguments(argv)
Output(Return value): 0
Brief description of the task:
*/
int main(int argc, char* argv[]){

    sem_init(&mutex, 0, 1);

    if(strcmp(argv[1], "-nodelay") == 1){
        if(argc < 5 || argc > 5){
            printf("-nodelay takes 3 arguments, vocalists, composers, and soundproof rooms\n");
            exit(1);
        }
        else{
            vocalists = atoi(argv[2]);
            composers = atoi(argv[3]);
            soundproof = atoi(argv[4]);
            pthread_t thread_id[vocalists+composers];
            for (int i =0; i < composers; i++)
                pthread_create(&thread_id[i], NULL, composers_task, (void *) i);
            for (int i =0; i < vocalists; i++)
                pthread_create(&thread_id[i], NULL, vocalists_task, (void *) i);
            for (int i =0; i <vocalists+composers; i++)
                pthread_join(thread_id[i], NULL);
        }
    }
    else if(strcmp(argv[1], "-randomdelay") == 1){
        if(argc < 7 || argc > 7){
            printf("-randomdelay takes 5 arguments, vocalists, composers, soundproof rooms, maxWanderTime, and maxSoundRoomUsageTime\n");
            exit(1);
        }
        else{
            vocalists = atoi(argv[2]);
            composers = atoi(argv[3]);
            soundproof = atoi(argv[4]);
            pthread_t thread_id[vocalists+composers];

            randWander = rand() % atoi(argv[5]);
            randRoomUsage = rand() % atoi(argv[6]);
            flag = 1;

            for (int i =0; i < composers; i++)
                pthread_create(&thread_id[i], NULL, composers_task, (void *) i);
            for (int i =0; i < vocalists; i++)
                pthread_create(&thread_id[i], NULL, vocalists_task, (void *) i);
            for (int i =0; i <vocalists+composers; i++)
                pthread_join(thread_id[i], NULL);
        }
    }
}

/*
Function Name: vocalists_task
Input to the method:
Output(Return value):
Brief description of the task:
*/
void vocalists_task(int i) {
    //Code for the thread task goes here
    sem_wait(&mutex);
    printf("Entered Vocalists thread: %d\n", i);

    if(flag == 1) {
        for(int i = 0; i > randWander; i++){}
    }
    else{

    }
    //critical section, where all the work is done for filling the soundproof rooms with 1 vocalist and 1 composer


    printf("Exiting Vocalists thread: %d\n", i);
    sem_post(&mutex);
    pthread_exit(0); // this code returns to the corresponding pthread_join issued in main()
}

/*
Function Name: composers_task
Input to the method:
Output(Return value):
Brief description of the task:
*/
void composers_task(int i) {
    //Code for the thread task goes here
    sem_wait(&mutex);
    printf("Entered Composers thread: %d\n", i);

    if(flag == 1) {
        for(int i = 0; i > randWander; i++){}
    }
    else{

    }

    printf("Exiting Composers thread: %d\n", i);
    sem_post(&mutex);
    pthread_exit(0); // this code returns to the corresponding pthread_join issued in main()
}