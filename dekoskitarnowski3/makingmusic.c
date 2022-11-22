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
int* randRoomUsage;
int* randWander;
int flag = 0;

void vocalists_task(int);
void composers_task(int);

/*
Function Name: main
Input to the method: Number of arguments(argc) and arguments(argv)
Output(Return value): 0
Brief description of the task:
 create the threads for the vocalists and composers
*/
int main(int argc, char* argv[]){

    sem_init(&mutex, 0, 1);
    printf("main()\n");

    if(strcmp(argv[1], "-nodelay")){
        printf("-nodelay\n");
        if(argc < 5 || argc > 5){
            printf("-nodelay takes 3 arguments, vocalists, composers, and soundproof rooms\n");
            exit(1);
        }
        else{
            vocalists = atoi(argv[2]);
            composers = atoi(argv[3]);
            soundproof = atoi(argv[4]);
            pthread_t thread_id[vocalists+composers];
            for (int i =0; i < composers; i++){
                pthread_create(&thread_id[i], NULL, composers_task, (void *) i);
            }
            for (int i =0; i < vocalists; i++) {
                pthread_create(&thread_id[i], NULL, vocalists_task, (void *) i);
            }
            for (int i =0; i <vocalists+composers; i++) {
                pthread_join(thread_id[i], NULL);
            }
        }
    }
    else if(strcmp(argv[1], "-randomdelay")){
        printf("-randomdelay\n");
        if(argc < 7 || argc > 7){
            printf("-randomdelay takes 5 arguments, vocalists, composers, soundproof rooms, maxWanderTime, and maxSoundRoomUsageTime\n");
            exit(1);
        }
        else{
            vocalists = atoi(argv[2]);
            composers = atoi(argv[3]);
            soundproof = atoi(argv[4]);
            pthread_t thread_id[vocalists+composers];

            flag = 1;

            randWander = malloc(vocalists+composers);
            randRoomUsage = malloc(sizeof(int)*(vocalists+composers));

            for (int i =0; i < composers; i++) {
                randWander[i] = rand() % atoi(argv[5]);
                randRoomUsage[i] = rand() % atoi(argv[6]);
                pthread_create(&thread_id[i], NULL, composers_task, (void *) i);
            }
            for (int i =0; i < vocalists; i++) {
                randWander[i+composers+1] = rand() % atoi(argv[5]);
                randRoomUsage[i+composers+1] = rand() % atoi(argv[6]);
                pthread_create(&thread_id[i+composers+1], NULL, vocalists_task, (void *) i);
            }
            for (int i =0; i <vocalists+composers; i++) {
                pthread_join(thread_id[i], NULL);
            }
        }
    }
    return 0;
}

/*
Function Name: vocalists_task
Input to the method: thread number as an int threadNum
Output(Return value): None
Brief description of the task:
 
*/
void vocalists_task(int threadNum) {
    //Code for the thread task goes here
    sem_wait(&mutex);
    printf("Vocalist %d: I am wandering...\n", threadNum);

    if(flag == 1) {
        printf("-randomdelay vocalists_task\n");
        for(int i = 0; i > randWander[threadNum]; i++){}
    }
    else{
        printf("-nodelay vocalists_task\n");
    }

    printf("Vocalist %d: I am leaving... Bye\n", threadNum);
    sem_post(&mutex);
    pthread_exit(0); // this code returns to the corresponding pthread_join issued in main()
}

/*
Function Name: composers_task
Input to the method: thread number as an int threadNum
Output(Return value): None
Brief description of the task:

*/
void composers_task(int threadNum) {
    //Code for the thread task goes here
    sem_wait(&mutex);
    printf("Composer %d: I am wandering...\n", threadNum);

    if(flag == 1) {
        printf("-randomdelay composers_task\n");
        for(int i = 0; i > randWander[threadNum]; i++){}
    }
    else{
        printf("-nodelay composers_task\n");
    }

    printf("Composer %d: I am leaving... Bye\n", threadNum);
    sem_post(&mutex);
    pthread_exit(0); // this code returns to the corresponding pthread_join issued in main()
}