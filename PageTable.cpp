#include "PageTable.h"
#include "Level.h"
#include <iostream>

using namespace std;

void PageTable::AllocateFirstLevel(PageTable* pgTable){
    // create level 0 page
    pgTable->RootLevelPtr = new Level();
    // level 0 points back to page table
    pgTable->RootLevelPtr->PageTablePtr = pgTable;
    // initialize depth to 0
    pgTable->RootLevelPtr->depth = 0;
    // resize current vpn array to number of levels
    pgTable->currVPN.resize(pgTable->levelCount);
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
PageTable::PageTable(){}