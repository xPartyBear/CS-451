/*
Author: Michael Dekoski & Evan Tarnowski
Assignment Number: 4
Date of Submission: 12-07-2022
Name of this file: demandpaging.c
Short description of contents:
*/

#include <stdio.h>
#include <stdlib.h>

int RAM[128];
int pageLength = 16;
int pages = 0;
int pageAccessed[8];
int pageTable[1000];
int pageNumber;
int pageOffset;
int frames = 32;
int address[1000];
char addressStr[10];
int addressInt = 0;
int intTemp[5];
int addressLength = 0;
int RAMIndex = 0;
int pageAddedCount = 0;
int fifoFaults = 0;
int lruFaults = 0;

int fifo();
int lru();

int main(int argc, char* argv[]){
    if(argc == 2){
        FILE* addressFile;
        addressFile = fopen(argv[1], "r");
        while(1){
            //gets one line from file
            if( fgets (addressStr, 10, addressFile)==NULL )
                break;
            //runs through each char to get every int
            for(int p = 0; p< 60 ;p++){
                if (addressStr[p] >= 0x30 && addressStr[p] <= 0x39 ){
                    intTemp[addressLength] = addressStr[p] - 0x30;
                    addressLength++; // counts the number of ints found
                }
            }
            switch(addressLength){//formats the integer correctly
                case 5:
                    address[addressInt] = intTemp[0] * 10000 + intTemp[1] * 1000 + intTemp[2] * 100 + intTemp[3] * 10 + intTemp[4];
                    break;
                case 4:
                    address[addressInt] = intTemp[0] * 1000 + intTemp[1] * 100 + intTemp[2] * 10 + intTemp[3];
                    break;

                case 3:
                    address[addressInt] = intTemp[0] * 100 + intTemp[1] * 10 + intTemp[2];
                    break;
                default:
                    break;
            }
            if(address[addressInt] == 0)
                addressInt--;
            addressLength = 0;
            addressInt++;
        }

        for(int i = 0; i < 1000; i++){
            pageTable[i] = -1;
        }

        fifoFaults = fifo();
        lruFaults = lru();

        printf("The number of page faults for FIFO was: %d\n", fifoFaults);
        printf("The number of page faults for LRU was: %d\n", lruFaults);

    }
    else{
        printf("The program takes one argument, the address file.\n");
        exit(0);
    }

}


// FIFO algorithm for paging
int fifo() {
    int faults = 0;
    int s = 0;
    for (int i = 0; i < 1000; i++) {
        pageNumber = address[i] / pageLength;
        pageOffset = address[i] % pageLength;
        RAMIndex = pageAddedCount;
        if (pageTable[pageNumber] == -1){
            if(pageAddedCount < 8){//RAM not full
                for(int k = 0; k < pageAddedCount; k++){ //add the MRU to the front of the array pushing everything else to the right by 1 index
                    pageAccessed[k] = pageAccessed[k-1];
                }
                pageAccessed[0] = pageNumber;
                pageTable[pageNumber] = pageAddedCount;
                for(int z = 0; z < 16;z++){ //load data to the ram 16 bytes or 1 page
                    RAM[(pageTable[pageNumber] * 16) + z] = 1;
                }
                pageAddedCount++;
            }
            else{//RAM Full
                pageTable[pageNumber] = pageTable[pageAccessed[0]];//set the pageTable for current pageNumber to the number used by the LRU
                pageTable[pageAccessed[0]] = -1;
                for(int y = 0; y < 8; y++){
                    pageAccessed[y] = pageAccessed[y-1];
                }
                pageAccessed[0] = pageNumber;
                for(int z = 0; z < 16;z++){ //load data to the ram 16 bytes or 1 page
                    RAM[(pageTable[pageNumber] * 16) + z] = 1;
                }

            }
            faults++;//counter for page faults
        }
        else{
            for(int a = 0; a < pageAddedCount; a++){
                if (pageAccessed[a] == pageNumber){ //check when the pageNumber was accessed
                    RAMIndex = a;//save the location
                    break;
                }
            }

            for(int b = 0; b < RAMIndex; b++) {//shift everything to the right up to the current pageNumbers location in the array
                pageAccessed[b] = pageAccessed[b - 1];
            }

            pageAccessed[0] = pageNumber; //add pageNumber to the front of the queue
        }
    }
    return faults;
}


// LRU algorithm for paging
int lru(){
    int faults = 0;

    return faults;
}