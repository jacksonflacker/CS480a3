#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H
#include <vector>
#include <stdint.h>
#include "Level.h"

using namespace std;

// typedef struct PageTable{
//         Level* RootLevelPtr;
//         int levelCount;
//         vector<unsigned int> BitmaskAry;
//         //uint32_t BitmaskAry[3];
//         vector<int> ShiftAry;
//         //int ShiftAry[3];
//         vector<uint32_t> EntryCount;
//         //uint32_t EntryCount[3];

// }PageTable;
// void AllocateFirstLevel(PageTable*);
class PageTable{
        public:
                class Level* RootLevelPtr;
                int levelCount;
                vector<unsigned int> BitmaskAry;
                vector<int> ShiftAry;
                vector<uint32_t> EntryCount;
                vector<int> SizeOfLevels;
                PageTable();
                void AllocateFirstLevel(PageTable*);
};
#endif
