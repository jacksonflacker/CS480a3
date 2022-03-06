#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include "Map.h"

using namespace std;

typedef struct Level{

    int depth;
    vector<struct Level*> NextLevelPtr;
    vector<Map> MAP;
}Level;

#endif