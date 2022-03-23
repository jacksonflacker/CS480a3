/**
 * @file Map.cpp
 * 
 * @author Omar Martinez
 * @author Jackson Flacker
 * 
 * @brief A structure containing information about the mapping of a page to a 
 *        frame, used in leaf nodes of the tree.  
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Map.h"
#include "Level.h"
#include <iostream>
#include <stdio.h>

using namespace std;

// default constructor
Map::Map(){}

/**
 * @brief Returns the mapping of virtual address to physical frame (in Map*) 
 *        from the page table. Walks page table until mapping found or mapping
 *        not in the page table.
 * 
 * @param pageTable: PageTable pointer
 * @param virtualAddress: current virtual address
 * @return Map*: pointer to Map object if found
 * @return NULL: otherwise
 */
Map* Map::pageLookUp(PageTable* pageTable, unsigned int virtualAddress){
    Level *currLevel = pageTable->RootLevelPtr;
    bool VPN_notFound = true;
    do{
        // extract page level VPN
        unsigned int VPN = virtualAddress & pageTable->BitmaskAry[currLevel->depth];
        VPN = VPN >> pageTable->ShiftAry[currLevel->depth];
        // check if current level is a leaf node
        if(currLevel->depth+1 == pageTable->levelCount){
            // check if logical address already maps to a PFN
            if(currLevel->MapPtr[VPN] != NULL){
                // increment page hit counter
                pageTable->pageHits++;
                return currLevel->MapPtr[VPN];
            }
            // set flag to false, need to insert into pagetable
            else{
                //increment page miss counter
                pageTable->pageMisses++;
                VPN_notFound = false;
            }
        }
        // if not leaf node, then interate through next level
        else{
            // check if next level is not null before iterating
            if(currLevel->NextLevelPtr[VPN] != NULL){
                currLevel = currLevel->NextLevelPtr[VPN];
            }
            // if null, set flag to false and exit loop
            else{
                //increment page miss counter
                pageTable->pageMisses++;
                VPN_notFound = false;
            }
        }
    }while(VPN_notFound);
    
    return NULL;
}