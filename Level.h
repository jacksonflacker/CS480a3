#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include "PageTable.h"

using namespace std;

// typedef struct Level{
//     int depth;
//     struct PageTable* PageTablePtr;
//     vector<Level*> NextLevelPtr;
// }Level;

class Level{
    public:
        int depth;
        class PageTable *PageTablePtr;
        vector<Level*> NextLevelPtr;
        Level();
};
#endif