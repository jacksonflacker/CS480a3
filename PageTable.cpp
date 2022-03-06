#include <iostream>
#include "PageTable.h"

using namespace std;

void initializePageTable(PageTable &pgTable){
    uint32_t BitmaskAry [] = {0xFF000000, 0x00FF0000, 0x0000FF00};
    uint32_t ShiftAry [] = {24, 16, 8};
    for(int i = 0; i < pgTable.LevelCount; i++){
        pgTable.BitmaskAry.push_back(BitmaskAry[i]);
        pgTable.ShiftAry.push_back(ShiftAry[i]);
    }
    pgTable.RootNodePtr = new Level();
}