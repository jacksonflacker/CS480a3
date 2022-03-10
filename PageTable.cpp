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
    // resize Next Level Array to page size
    pgTable->RootLevelPtr->NextLevelPtr.resize(pgTable->EntryCount[0]);
    // resize Next Level Array to page size
    pgTable->RootLevelPtr->MapPtr.resize(pgTable->EntryCount[0]);
}
PageTable::PageTable(){}