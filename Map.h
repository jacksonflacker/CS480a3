/**
 * @file Map.h
 * 
 * @author Omar Martinez
 * @author Jackson Flacker
 * 
 * @brief A structure containing information about the mapping of a page to a 
 *        frame, used in leaf nodes of the tree.  
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MAP_H
#define MAP_H
#include "PageTable.h"

class Map{
    public:
        Map();
        Map* pageLookUp(class PageTable*, unsigned int); // page table walk
        uint32_t VPN;   // virtual page number
        uint32_t PFN;   // physical frame number
        bool valid;
};
#endif