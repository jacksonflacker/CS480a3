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
        // extract page level VPN
        //printf("virtual address: %x\n",virtualAddress);
        unsigned int VPN = virtualAddress & pageTable->BitmaskAry[currLevel->depth];
        VPN = VPN >> pageTable->ShiftAry[currLevel->depth];
        //printf("vitrual page number: %x\n",VPN);
        // check if current level is a leaf node
        if(currLevel->depth+1 == pageTable->levelCount){
            // check if logical address already maps to a PFN
            if(currLevel->MapPtr[VPN] != NULL)
                return currLevel->MapPtr[VPN];
            // set flag to false, need to insert into pagetable
            else
                VPN_notFound = false;
        }
        // if not leaf node, then interate through next level
        else{
            // check if next level is not null before iterating
            if(currLevel->NextLevelPtr[VPN] != NULL){
                currLevel = currLevel->NextLevelPtr[VPN];
            }
            // if null, set flag to false and exit loop
            else{
                VPN_notFound = false;
            }
        }
    }while(VPN_notFound);
    
    return NULL;
}