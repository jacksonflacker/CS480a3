#ifndef LEVEL_H
#define LEVEL_H
#include <vector>


typedef struct{

    int depth;
    vector<Level*> NextLevelPtr;

}Level;

#endif