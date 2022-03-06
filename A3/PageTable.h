#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H
#include <vector>
#include "Level.h"

typedef struct{
    Level *RootNodePtr;
    int LevelCount;
    vector<uint32_t> BitmaskAry;
    vector<int> ShiftAry;
    vector<uint32_t> EntryCount;

}PageTable;

#endif