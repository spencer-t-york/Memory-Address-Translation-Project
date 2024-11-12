#include "phyframe.h"
#include <stdio.h>

int findIndex(int pageTable[], int virtualPageNumber) {
    for (int i = 1; i < 8; i++) {
        if (pageTable[i] == virtualPageNumber) {
            return i;
        }
    }
    return -1;
}

int findOpenSpot(int pageTable[]) {
    for (int i = 1; i < 8; i++) {
        if (pageTable[i] == -1) {
            return i;
        }
    }
    return -1;
}

int getPhysicalFrameNumber(int pageTable[], Stack *LRUstack, int virtualPageNumber, int *pageFaultCounter) {
    int physicalFrameNumber = findIndex(pageTable, virtualPageNumber);
    if (physicalFrameNumber != -1) { // if virtual page number is in page table
        accessPage(LRUstack, virtualPageNumber);
        printPageTable(pageTable);
        return physicalFrameNumber;  // return the physical frame number
    }                         // if virtual page number is not in page table
    (*pageFaultCounter)++;
    int openSpot = findOpenSpot(pageTable);
    if (openSpot != -1) {       // if there is an open spot in page table
        pageTable[openSpot] = virtualPageNumber;
        accessPage(LRUstack, virtualPageNumber);
        printPageTable(pageTable);
        return openSpot;        // return the physical frame number
    }

    // if there are no open spots in page table
    int LRUVirtualPage = LRUstack->virtualPageNumbers[0];
    int frameToReplace = -1;

    for (int j = 1; j < 8; j++) {
        if (pageTable[j] == LRUVirtualPage) {
            frameToReplace = j;
            break;
        }
    }

    if (frameToReplace != -1) {
        pageTable[frameToReplace] = virtualPageNumber;
        accessPage(LRUstack, virtualPageNumber);
        printPageTable(pageTable);
        return frameToReplace;       // return the physical frame number
    }

    printf("\n!!!!!!!!!!\nSomething went wrong when getting the physical frame number.\n");
    return -1; // in case something goes really wrong
}
