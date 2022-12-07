/*
Author: Michael Dekoski & Evan Tarnowski
Assignment Number: 4
Date of Submission: 12-07-2022
Name of this file: demandpaging.c
Short description of contents:
 using FIFO and LRU convert the logical addresses to physical addresses
*/

#include <stdio.h>
#include <stdlib.h>

int pageLength = 1024;//in bytes, this is 1kb
int* pageTable;
int address[1000];
int addresses = 0;

int fifo();

int lru();

/*
Function Name: main
Input to the method: Number of arguments(argc) and arguments(argv)
Output(Return value): 0
Brief description of the task:
 formats the addresses to be the same length
 then runs FIFO and LRU paging algorithms
*/
int main(int argc, char *argv[]) {
    if (argc == 2) {
        printf("The logical addresses corresponding to physical \n"
               "addresses given in addresses.txt when using FIFO and \n"
               "LRU algorithms are available in files \n"
               "FIFO_PhysicalAddresses.txt and \n"
               "LRU_PhysicalAddresses.txt.\n\n");
        FILE *addressFile;
        addressFile = fopen(argv[1], "r");

        int intTemp[5];
        int addressInt = 0;
        int addressLength = 0;
        char addressStr[60];
        while (1) {
            if (fgets(addressStr, 10, addressFile) == NULL)// get logical addresses from file
                break;
            addresses++;
            for (int p = 0; p < 60; p++) {// get logical address digits
                if (addressStr[p] >= 0x30 && addressStr[p] <= 0x39) {
                    intTemp[addressLength] = addressStr[p] - 0x30;
                    addressLength++;
                }
            }

            switch (addressLength) {// format all logical address to have 5 digits
                case 5:
                    address[addressInt] =
                            intTemp[0] * 10000 + intTemp[1] * 1000 + intTemp[2] * 100 + intTemp[3] * 10 + intTemp[4];
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
            if (address[addressInt] == 0)
                addressInt--;
            addressLength = 0;
            addressInt++;
        }
        pageTable = calloc(addresses, sizeof(int));

        for (int i = 0; i < addresses; i++) {// set page table to all -1
            pageTable[i] = -1;
        }
        fclose(addressFile);

        int fifoFaults;
        int lruFaults;
        fifoFaults = fifo();// run FIFO algorithm
        lruFaults = lru();// run LRU algorithm

        printf("The number of page faults for FIFO was: %d\n", fifoFaults);
        printf("The number of page faults for LRU was: %d\n", lruFaults);

    } else {
        printf("The program takes one argument, the address file.\n");
        exit(0);
    }

}


/*
Function Name: fifo
Input to the method: None
Output(Return value): faults
Brief description of the task:
 convert the logical addresses into physical addresses using the FIFO paging algorithm
*/
int fifo() {
    int faults = 0;
    int RAM[8196];
    int RAMIndex;
    int pageAccessed[8];
    int pageNumber;
    int pageOffset;
    int pageAddedCount = 0;

    FILE *FIFOStream;
    FIFOStream = fopen("FIFO_PhysicalAddresses.txt", "w");// file to store FIFO physical addresses
    for (int i = 0; i < (addresses); i++) {
        pageNumber = address[i] / pageLength;
        pageOffset = address[i] % pageLength;
        RAMIndex = pageAddedCount;
        if (pageTable[pageNumber] == -1) {
            if (pageAddedCount < 8) {//RAM not full
                for (int k = 0; k < pageAddedCount; k++) {// most recent page is moved to front of array
                    pageAccessed[k] = pageAccessed[k - 1];
                }
                pageAccessed[0] = pageNumber;// current page becomes most recent
                pageTable[pageNumber] = pageAddedCount;//add frame to page table
                for (int z = 0; z < pageLength; z++) {
                    RAM[(pageTable[pageNumber] * pageLength) + z] = 1;// fill 1kb of RAM in the frame
                }
                pageAddedCount++;
            } else {//RAM Full
                pageTable[pageNumber] = pageTable[pageAccessed[0]];
                pageTable[pageAccessed[0]] = -1;
                for (int y = 0; y < 8; y++) {
                    pageAccessed[y] = pageAccessed[y - 1];
                }
                pageAccessed[0] = pageNumber;
                for (int z = 0; z < pageLength; z++) {
                    RAM[(pageTable[pageNumber] * pageLength) + z] = 1;// fill 1kb of RAM in the frame
                }
            }
            faults++;// fault happened
        } else {
            for (int a = 0; a < pageAddedCount; a++) {
                if (pageAccessed[a] == pageNumber) {
                    RAMIndex = a;
                    break;
                }
            }
            for (int b = 0; b < RAMIndex; b++) {// moves pages to left until current page
                pageAccessed[b] = pageAccessed[b - 1];
            }
            pageAccessed[0] = pageNumber;// current page becomes most recent
        }
        fprintf(FIFOStream, "%d\n", pageTable[pageNumber]*pageLength + pageOffset);
    }
    fclose(FIFOStream);
    return faults;
}


/*
Function Name: lru
Input to the method: None
Output(Return value): faults
Brief description of the task:
 convert the logical addresses into physical addresses using the LRU paging algorithm
*/
int lru() {
    int faults = 0;
    int RAM[8196];
    int RAMIndex;
    int pageAccessed[8];
    int pageNumber;
    int pageOffset;
    int pageAddedCount = 0;

    FILE *LRUStream;
    LRUStream = fopen("LRU_PhysicalAddresses.txt", "w");// file to store LRU physical addresses
    for (int i = 0; i < (addresses); i++) {
        pageNumber = address[i] / pageLength;
        pageOffset = address[i] % pageLength;
        RAMIndex = pageAddedCount;
        if (pageTable[pageNumber] == -1) {
            if (pageAddedCount < 8) {//RAM not full
                for (int k = pageAddedCount; k > 0; k--) {// most recent page is moved to front of array
                    pageAccessed[k] = pageAccessed[k - 1];
                }
                pageAccessed[0] = pageNumber;// current page becomes most recent
                pageTable[pageNumber] = pageAddedCount;// add frame to page table
                for (int z = 0; z < pageLength; z++) {
                    RAM[(pageTable[pageNumber] * pageLength) + z] = 1;// fill 1kb of RAM in the frame
                }
                pageAddedCount++;
            } else {//RAM Full
                pageTable[pageNumber] = pageTable[pageAccessed[7]];
                pageTable[pageAccessed[7]] = -1;
                for (int y = 8; y > 0; y--) {
                    pageAccessed[y] = pageAccessed[y - 1];
                }
                pageAccessed[0] = pageNumber;
                for (int z = 0; z < pageLength; z++) {
                    RAM[(pageTable[pageNumber] * pageLength) + z] = 1;// fill 1kb of RAM in the frame
                }
            }
            faults++;// fault happened
        } else {
            for (int a = 0; a < pageAddedCount; a++) {
                if (pageAccessed[a] == pageNumber) {
                    RAMIndex = a;
                    break;
                }
            }
            for (int b = RAMIndex; b > 0; b--) {// move pages to right until the current page
                pageAccessed[b] = pageAccessed[b - 1];
            }
            pageAccessed[0] = pageNumber;// current page becomes most recent
        }
        fprintf(LRUStream, "%d\n", pageTable[pageNumber]*pageLength + pageOffset);
    }
    fclose(LRUStream);
    return faults;
}