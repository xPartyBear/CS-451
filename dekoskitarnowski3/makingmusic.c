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
int randRoomUsage;
int randWander;
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

    if(strcmp(argv[1], "-nodelay") == 0){//if "-nodelay" is used
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

            for (int i =0; i < composers; i++) {//create composer threads
                pthread_create(&composers_thread_id[i], NULL, (void*)&composers_task, (void *)i);
            }
            for (int i =0; i < vocalists; i++) {//create vocalist thread
                pthread_create(&vocalists_thread_id[i], NULL, (void*)&vocalists_task, (void *)i);
            }
            while(vocalists != vocalistThreadCount){}//wait until all vocalist threads are wandering
            sem_post(&vocalist);//allow to make music
            while(composers != composerThreadCount){}//wait until all composer threads are wandering
            sem_post(&composer);//allow to make music
            for (int i =0; i <vocalists; i++) {//kill vocalist threads when they are done
                pthread_join(vocalists_thread_id[i], NULL);
            }
            for (int i =0; i <composers; i++) {//kill composer threads when they are done
                pthread_join(composers_thread_id[i], NULL);
            }
        }
    }
    else if(strcmp(argv[1], "-randomdelay") == 0){//if "-randomdelay" is used
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

            randWander = rand() % (atoi(argv[5])+1);//get the random wander time
            randRoomUsage = rand() % (atoi(argv[6])+1);//get the random room usage time

            for (int i =0; i < composers; i++) {//create composer threads
                pthread_create(&composers_thread_id[i], NULL, (void*)&composers_task, (void *)i);
            }
            for (int i =0; i < vocalists; i++) {//create vocalist thread
                pthread_create(&vocalists_thread_id[i], NULL, (void*)&vocalists_task, (void *)i);
            }
            while(vocalists != vocalistThreadCount){}//wait until all vocalist threads are wandering
            sem_post(&vocalist);//allow to make music
            while(composers != composerThreadCount){}//wait until all composer threads are wandering
            sem_post(&composer);//allow to make music
            for (int i =0; i <vocalists; i++) {//kill vocalist threads when they are done
                pthread_join(vocalists_thread_id[i], NULL);
            }
            for (int i =0; i <composers; i++) {//kill composer threads when they are done
                pthread_join(composers_thread_id[i], NULL);
            }
        }
    }
    else{//if neither are used
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
        sleep(randWander);//wander for randWander
        printf("Vocalist %d: I am ready to make music.\n", threadNum);
        sem_post(&vocalist);//vocalist can be used to make music
        currentVocalist = threadNum;

        while(1){//wait for a room to be available
            currentVocalist = threadNum;
            sem_wait(&room);
            if(soundproof >= 0){//if we have rooms open use a room
                --soundproof;
                sem_post(&room);//open the room for a composer
                sem_post(&composer);//get a composer
                sem_wait(&vocalist);//get the current vocalist
                sem_wait(&room);//close the room
                printf("Vocalist %d and Composer %d found a soundproof room and are making music.\n", currentVocalist,
                       currentComposer);
                sleep(randRoomUsage);//use the room with a time of randRoomUsage
                soundproof++;
                sem_post(&room);//open the room
                break;
            }
            else{
                sem_post(&room);
            }
        }
    }
    else{
        printf("Vocalist %d: I am wandering...\n", threadNum);
        sem_wait(&vocalist);
        printf("Vocalist %d: I am ready to make music.\n", threadNum);
        currentVocalist = threadNum;

        while(1){//wait for a room to be available
            currentVocalist = threadNum;
            sem_wait(&room);
            if(soundproof >= 0){//if we have rooms open use a room
                --soundproof;
                sem_post(&room);//open the room for a composer
                sem_post(&composer);//get a composer
                sem_wait(&vocalist);//get the current vocalist
                sem_wait(&room);//close the room
                printf("Vocalist %d and Composer %d found a soundproof room and are making music.\n", currentVocalist,
                       currentComposer);
                soundproof++;
                sem_post(&room);//open the room
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
        sleep(randWander);//wander for randWander
        printf("Composer %d: I am ready to make music.\n", threadNum);
        sem_post(&composer);//composer can be used to make music
        currentComposer = threadNum;

        while(1){//wait for a room to be available
            currentComposer = threadNum;
            sem_wait(&room);
            if(soundproof >= 0){//if we have rooms open use a room
                --soundproof;
                sem_post(&room);//open the room for a vocalist
                sem_post(&vocalist);//get a vocalist
                sem_wait(&composer);//get the current composer
                sem_wait(&room);//close the room
                sleep(randRoomUsage);//use the room for randRoomUsage
                soundproof++;
                sem_post(&room);//open the room
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

        while(1){//wait for a room to be available
            currentComposer = threadNum;
            sem_wait(&room);
            if(soundproof >= 0){//if we have rooms open use a room
                --soundproof;
                sem_post(&room);//open the room for a vocalist
                sem_post(&vocalist);//get a vocalist
                sem_wait(&composer);//get the current composer
                sem_wait(&room);//close the room
                soundproof++;
                sem_post(&room);//open the room
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