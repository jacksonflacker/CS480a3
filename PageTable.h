/**
 * @file PageTable.h
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
#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H
#include <vector>
#include <stdint.h>
#include "Level.h"

using namespace std;

class PageTable{
public:
        class Level* RootLevelPtr; // pointer to level 0 
        int levelCount; // number of levels in page table
        int offset; // offset bits
        uint32_t pageHits, pageMisses;
        long bytesUsed;
        vector<uint32_t> currVPN; // multi level VPNs of current virtual address
        vector<unsigned int> BitmaskAry; // bitmask of each level
        vector<int> ShiftAry; // shift count of each level
        vector<uint32_t> EntryCount; // number of entries for each level
        vector<int> SizeOfLevels; // bits each level occupies
        PageTable();
        void AllocateFirstLevel(PageTable*);
        void pageInsert(unsigned int, unsigned int&);
};
#endif
