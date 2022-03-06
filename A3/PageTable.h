#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H
#include <vector>
#include "Level.h"
using namespace std;

typedef struct{
    Level *RootNodePtr;
    int LevelCount;
    int offset;
    vector<uint32_t> BitmaskAry;
    vector<int> ShiftAry;
    vector<uint32_t> EntryCount;

}PageTable;

#endif