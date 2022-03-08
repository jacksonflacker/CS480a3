#ifndef LEVEL_H
#define LEVEL_H
#include <vector>

using namespace std;

struct Level{
    int depth;
    vector<Level*> NextLevelPtr;

};

#endif