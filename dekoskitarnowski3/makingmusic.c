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

sem_t composer;
sem_t vocalist;
sem_t room;

int vocalists;
int composers;
int soundproof;
int* randRoomUsage;
int* randVocalistWander;
int* randComposerWander;
int flag = 0;

int currentVocalist = 0;
int currentComposer = 0;

int vocalistThreadCount = 0;
int composerThreadCount = 0;

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

    sem_init(&composer, 0, 0);
    sem_init(&vocalist, 0, 0);
    sem_init(&room, 0, 1);

    if(strcmp(argv[1], "-nodelay") == 0){
        if(argc < 5 || argc > 5){
            printf("-nodelay takes 3 arguments, vocalists, composers, and soundproof rooms\n");
            exit(1);
        }
        else{
            vocalists = atoi(argv[2]);
            composers = atoi(argv[3]);
            soundproof = atoi(argv[4]);
            pthread_t vocalists_thread_id[vocalists];
            pthread_t composers_thread_id[composers];
            for (int i =0; i < composers; i++){
                pthread_create(&composers_thread_id[i], NULL, (void*)&composers_task, (void *)i);
            }
            for (int i =0; i < vocalists; i++) {
                pthread_create(&vocalists_thread_id[i], NULL, (void*)&vocalists_task, (void *)i);
            }
            while(vocalists != vocalistThreadCount){}
            sem_post(&vocalist);
            while(composers != composerThreadCount){}
            sem_post(&composer);
            for (int i =0; i <vocalists; i++) {
                pthread_join(vocalists_thread_id[i], NULL);
            }
            for (int i =0; i <composers; i++) {
                pthread_join(composers_thread_id[i], NULL);
            }
        }
    }
    else if(strcmp(argv[1], "-randomdelay") == 0){
        if(argc < 7 || argc > 7){
            printf("-randomdelay takes 5 arguments, vocalists, composers, soundproof rooms, maxWanderTime, and maxSoundRoomUsageTime\n");
            exit(1);
        }
        else{
            vocalists = atoi(argv[2]);
            composers = atoi(argv[3]);
            soundproof = atoi(argv[4]);
            pthread_t vocalists_thread_id[vocalists];
            pthread_t composers_thread_id[composers];

            flag = 1;

            randVocalistWander = malloc(sizeof(int)*vocalists);
            randComposerWander = malloc(sizeof(int)*composers);
            randRoomUsage = malloc(sizeof(int)*vocalists);

            for (int i =0; i < composers; i++) {
                randComposerWander[i] = rand() % (atoi(argv[5])+1);
                pthread_create(&composers_thread_id[i], NULL, (void*)&composers_task, (void *)i);
            }
            for (int i =0; i < vocalists; i++) {
                randVocalistWander[i] % (atoi(argv[5])+1);
                randRoomUsage[i] = rand() % (atoi(argv[6])+1);
                pthread_create(&vocalists_thread_id[i], NULL, (void*)&vocalists_task, (void *)i);
            }
            while(vocalists != vocalistThreadCount){}
            sem_post(&vocalist);
            while(composers != composerThreadCount){}
            sem_post(&composer);
            for (int i =0; i <vocalists; i++) {
                pthread_join(vocalists_thread_id[i], NULL);
            }
            for (int i =0; i <composers; i++) {
                pthread_join(composers_thread_id[i], NULL);
            }
        }
    }
    else{
        printf("Unknown command\n");
    }
    return 0;
}

/*
Function Name: vocalists_task
Input to the method: thread number as an int threadNum
Output(Return value): None
Brief description of the task:
 set and post mutexes to have composer and vocalist in a soundproof room
*/
void vocalists_task(int threadNum) {
    vocalistThreadCount++;
    //Code for the thread task goes here
    currentVocalist = threadNum;
    if(flag == 1) {
        printf("Vocalist %d: I am wandering...\n", threadNum);
        sem_wait(&vocalist);
        sleep(randVocalistWander[threadNum]);
        printf("Vocalist %d: I am ready to make music.\n", threadNum);
        currentVocalist = threadNum;

        while(1){
            currentVocalist = threadNum;
            sem_wait(&room);
            if(soundproof >= 0){
                --soundproof;
                sem_post(&room);
                sem_post(&composer);
                sem_wait(&vocalist);
                sem_wait(&room);
                printf("Vocalist %d and Composer %d found a soundproof room and are making music.\n", currentVocalist,
                       currentComposer);
                sleep(randRoomUsage[threadNum]);
                soundproof++;
                sem_post(&room);
                break;
            }
            else{
                sem_post(&room);
            }
        }

        printf("Vocalist %d and Composer %d found a soundproof room and are making music.\n", currentVocalist,
               currentComposer);
        sleep(randRoomUsage[threadNum]);
        sem_post(&room);
        sem_post(&composer);
    }
    else{
        printf("Vocalist %d: I am wandering...\n", threadNum);
        sem_wait(&vocalist);
        printf("Vocalist %d: I am ready to make music.\n", threadNum);
        currentVocalist = threadNum;

        while(1){
            currentVocalist = threadNum;
            sem_wait(&room);
            if(soundproof >= 0){
                --soundproof;
                sem_post(&room);
                sem_post(&composer);
                sem_wait(&vocalist);
                sem_wait(&room);
                printf("Vocalist %d and Composer %d found a soundproof room and are making music.\n", currentVocalist,
                       currentComposer);
                soundproof++;
                sem_post(&room);
                break;
            }
            else{
                sem_post(&room);
            }
        }
    }

    printf("Vocalist %d: I am leaving... Bye\n", threadNum);
    sem_post(&vocalist);
    pthread_exit(0); // this code returns to the corresponding pthread_join issued in main()
}

/*
Function Name: composers_task
Input to the method: thread number as an int threadNum
Output(Return value): None
Brief description of the task:
 set and post mutexes to have composer and vocalist in a soundproof room
*/
void composers_task(int threadNum) {
    composerThreadCount++;
    //Code for the thread task goes here
    currentComposer = threadNum;
    if(flag == 1) {
        printf("Composer %d: I am wandering...\n", threadNum);
        sem_wait(&composer);
        sleep(randComposerWander[threadNum]);
        printf("Composer %d: I am ready to make music.\n", threadNum);
        currentComposer = threadNum;

        while(1){
            currentComposer = threadNum;
            sem_wait(&room);
            if(soundproof >= 0){
                --soundproof;
                sem_post(&room);
                sem_post(&vocalist);
                sem_wait(&composer);
                sem_wait(&room);
//                printf("Vocalist %d and Composer %d found a soundproof room and are making music.\n", currentVocalist,
//                       currentComposer);
                sleep(randRoomUsage[currentVocalist]);
                soundproof++;
                sem_post(&room);
                break;
            }
            else{
                sem_post(&room);
            }
        }
    }
    else{
        printf("Composer %d: I am wandering...\n", threadNum);
        sem_wait(&composer);
        printf("Composer %d: I am ready to make music.\n", threadNum);
        currentComposer = threadNum;

        while(1){
            currentComposer = threadNum;
            sem_wait(&room);
            if(soundproof >= 0){
                --soundproof;
                sem_post(&room);
                sem_post(&vocalist);
                sem_wait(&composer);
                sem_wait(&room);
//                printf("Vocalist %d and Composer %d found a soundproof room and are making music.\n", currentVocalist,
//                       currentComposer);
                soundproof++;
                sem_post(&room);
                break;
            }
            else{
                sem_post(&room);
            }
        }
    }

    printf("Composer %d: I am leaving... Bye\n", threadNum);
    sem_post(&composer);
    pthread_exit(0); // this code returns to the corresponding pthread_join issued in main()
}