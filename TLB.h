#ifndef TLB_H
#define TLB_H
#include <map>
#include <vector>

using namespace std;

class TLB{
public:
    TLB();
    void insertIntoTLB(uint32_t,uint32_t);
    void LastRecentlyUsed(uint32_t);
    map<uint32_t,uint32_t> VPN2PFN;
    map<uint32_t,uint32_t> VPN_time;
    vector<uint32_t> LRU_10;
    int maxCapacity;
    uint32_t cacheHits, cacheMisses;
};

#endif