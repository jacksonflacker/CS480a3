/**
 * @file Level.h
 * 
 * @author Omar Martinez
 * @author Jackson Flacker
 * 
 * @brief An entry for an arbitrary level, this is the structure 
 *        which represents one of the sublevels in the page tree/table. 
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include "PageTable.h"
#include "Map.h"

using namespace std;

class Level{
    public:
        int depth;  // depth of level
        class PageTable *PageTablePtr; // pointer to page table
        vector<Level*> NextLevelPtr;    // array of next level pointers
        vector<class Map*> MapPtr;  // array of map object pointers
        Level();
};
#endif