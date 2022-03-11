#ifndef MAP_H
#define MAP_H
#include "PageTable.h"

// typedef struct Map{
//     uint32_t VPN;
//     uint32_t FPN;
//     bool flag;
//     uint32_t frame;
// }Map;

class Map{
    public:
        Map();
        Map* pageLookUp(class PageTable*, unsigned int);
        uint32_t VPN;
        uint32_t PFN;
        bool valid;
};
#endif