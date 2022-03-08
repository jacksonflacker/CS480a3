#include "PageTable.h"

using namespace std;

void AllocateFirstLevel(PageTable* pgTable){
    pgTable->RootLevelPtr = new Level();
    pgTable->RootLevelPtr->depth = 0;
    pgTable->RootLevelPtr->NextLevelPtr.resize(pgTable->EntryCount[0]);
}