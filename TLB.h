/**
 * @file TLB.h
 * 
 * @author Omar Martinez
 * @author Jackson Flacker
 * 
 * @brief MMU translation simulation for caching the virtual page to physical 
 *        frame mappings from the page table. TLB entry caches one mapping of a 
 *        Virtual Page Number to a Physical Frame Number.
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef TLB_H
#define TLB_H
#include <map>
#include <vector>

using namespace std;

class TLB{
public:
    TLB();
    void insertIntoTLB(uint32_t,uint32_t); // adds entries into cache
    void RecentlyUsed(uint32_t);    // monitors recently used addresses
    void TLB_insertion(uint32_t, uint32_t);
    void LRU_handler(uint32_t);
    map<uint32_t, uint32_t>VPN_to_PFN;
    map<uint32_t, int> VPN_virtualTime;
    map<uint32_t,uint32_t> VPN2PFN; // VPN to PFN mapping
    vector<uint32_t> LRU_10;    // array of recently used addresses
    int maxCapacity;    // max capacity of cached addresses 
    int virtualTime;
    uint32_t cacheHits, cacheMisses;
};

#endif