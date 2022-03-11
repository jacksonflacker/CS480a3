#include "Map.h"
#include "Level.h"
#include <iostream>
#include <stdio.h>

using namespace std;

Map::Map(){}

Map* Map::pageLookUp(PageTable* pageTable, unsigned int virtualAddress){
    Level *currLevel = pageTable->RootLevelPtr;
    bool VPN_notFound = true;
    do{
        printf("virtual address: %x\n",virtualAddress);
        unsigned VPN = virtualAddress & pageTable->BitmaskAry[currLevel->depth];
        VPN = VPN >> pageTable->ShiftAry[currLevel->depth];
        printf("vitrual page number: %x\n",VPN);
        // leaf node
        if(currLevel->depth+1 == pageTable->levelCount){
            if(currLevel->MapPtr[VPN] != NULL)
                return currLevel->MapPtr[VPN];
            else
                VPN_notFound = false;
        }
        else{
            if(currLevel->NextLevelPtr[VPN] != NULL){
                currLevel = currLevel->NextLevelPtr[VPN];
            }
            else{
                VPN_notFound = false;
            }
        }
    }while(VPN_notFound);
    
    return NULL;
}