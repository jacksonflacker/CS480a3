#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H
#include <vector>
#include "Level.h"

using namespace std;

typedef struct{
        Level* RootLevelPtr;
        int levelCount;
        vector<uint32_t> BitmaskAry;
        //uint32_t BitmaskAry[3];
        vector<int> ShiftAry;
        //int ShiftAry[3];
        vector<uint32_t> EntryCount;
        //uint32_t EntryCount[3];

}PageTable;
#endif
void AllocateFirstLevel(PageTable*);
