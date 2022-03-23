/**
 * @file PageTable.cpp
 * 
 * @author Omar Martinez
 * @author Jackson Flacker
 * 
 * @brief Top level descriptor describing attributes of the N level page table 
 *        and containing a pointer to the level 0 page tree/table structure.
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "PageTable.h"
#include "Level.h"
#include <iostream>

using namespace std;

/**
 * @brief Allocates level 0 to page table. Initializes all variables of level 0
 * 
 * @param pgTable: PageTable pointer
 */
void PageTable::AllocateFirstLevel(PageTable* pgTable){
    // create level 0 page
    pgTable->RootLevelPtr = new Level();
    // level 0 points back to page table
    pgTable->RootLevelPtr->PageTablePtr = pgTable;
    // initialize depth to 0
    pgTable->RootLevelPtr->depth = 0;
    // resize current vpn array to number of levels
    pgTable->currVPN.resize(pgTable->levelCount);
    
    // initialize bytes used
    pgTable->bytesUsed = 8 * pgTable->EntryCount[pgTable->RootLevelPtr->depth];
    pgTable->bytesUsed += 8 * sizeof(pgTable);

    // initialize page hit and miss counter
    pgTable->pageHits = 0;
    pgTable->pageMisses = 0;

    // check if first level is leaf
    if(pgTable->levelCount == 1){
        // resize map Array to page size
        pgTable->RootLevelPtr->MapPtr.resize(pgTable->EntryCount[0]);
    }
    // first level is not leaf
    else{
        // resize Next Level Array to page size
        pgTable->RootLevelPtr->NextLevelPtr.resize(pgTable->EntryCount[0]);
    }
}
/**
 * @brief Used to add new entries to the page table when we have discovered that
 *        a page has not yet been allocated. Creates new levels and/or iterates through
 *        levels until leaf node located. At leaf node, maps VPN to frame.
 * 
 * @param virtualAddress: current virtual address processed
 * @param frame: current frame number
 */
void PageTable::pageInsert(unsigned int virtualAddress, unsigned int &frame){
    // pointer to current level
    Level *currLevel = this->RootLevelPtr;
    // flag to end while loop
    bool VPN_notFound = true;
    // VPN used for mapping
    unsigned int fullVPN = 0;
    do{
        // masking and shifting to extract VPN from logical address
        unsigned int VPN = virtualAddress & this->BitmaskAry[currLevel->depth];
        VPN = VPN >> this->ShiftAry[currLevel->depth];
        // concatenate multi level vpns to generate full VPN
        fullVPN += VPN;
        // store multi level vpn into array of current VPNs processed
        this->currVPN[currLevel->depth] = VPN;
        // check if leaf node
        if(currLevel->depth+1 == this->levelCount){
            //check if map object is null
            if(currLevel->MapPtr[VPN] == NULL){
                //insert map pointer
                currLevel->MapPtr[VPN] = new Map();
                // store full virtual page number into map
                currLevel->MapPtr[VPN]->VPN = fullVPN;
                // insert frame and increment frame number after insert
                currLevel->MapPtr[VPN]->PFN = frame++;
                currLevel->MapPtr[VPN]->valid = true;
            }
            //set flag to false
            VPN_notFound = false;
        }
        // current level is not leaf node
        else{
            //check if level pointer at current vpn is null
            if(currLevel->NextLevelPtr[VPN] == NULL){
                // insert a new level pointer
                currLevel->NextLevelPtr[VPN] = new Level();
                // increment bytes used
                this->bytesUsed += (this->EntryCount[currLevel->depth+1]*8);
                // initialize new level depth
                currLevel->NextLevelPtr[VPN]->depth = currLevel->depth+1;
                // new level points back to pagetable
                currLevel->NextLevelPtr[VPN]->PageTablePtr = this;
                int currDepth = currLevel->NextLevelPtr[VPN]->depth;
                //check if new level is a leaf node
                if(currDepth+1 == this->levelCount){
                    // resize map array
                    currLevel->NextLevelPtr[VPN]->MapPtr.resize(this->EntryCount[currDepth]);                    
                }
                else{
                    // resize next level array
                    currLevel->NextLevelPtr[VPN]->NextLevelPtr.resize(this->EntryCount[currDepth]);
                }
            }
            // iterate through next level
            currLevel = currLevel->NextLevelPtr[VPN];
        }
    }while(VPN_notFound);
}

PageTable::PageTable(){}