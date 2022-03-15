#include "TLB.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <utility>

using namespace std;

TLB::TLB(){
    cacheHits = 0;
    cacheMisses = 0;
}

void TLB::insertIntoTLB(uint32_t VPN, uint32_t PFN){
    if(maxCapacity > 0){
        // check if cache size is less than max capacity
        if(VPN2PFN.size() < maxCapacity){
            // insert VPN as key mapped to PFN value
            VPN2PFN[VPN] = PFN;
            // insert VPN as key to integer representing time
            //VPN_time[VPN] = 0;
            // insert VPN into queue of 10 last recently used addresses
            LRU_10.push_back(VPN);
        }
        // remove 10th Last Recently Used mapping from table
        else{
            for(int i = 0; i < LRU_10.size(); i++){
                // if LRU address is inside the hash table, remove it
                if(VPN2PFN.count(LRU_10[i]) > 0){
                    // remove last recently used address from TLB
                    VPN2PFN.erase(LRU_10[i]);
                    break;
                }
            }
            // update LRU queue
            LastRecentlyUsed(VPN);
            // insert new address into map
            VPN2PFN[VPN] = PFN;
        }
    }
}
void TLB::LastRecentlyUsed(uint32_t VPN){
    // insert virtual address into queue
    LRU_10.push_back(VPN);
    // remove first element in queue
    if(LRU_10.size() > 10){
        LRU_10.erase(LRU_10.begin());
    }
}