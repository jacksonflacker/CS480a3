/**
 * @file TLB.cpp
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
#include "TLB.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <utility>

using namespace std;

// default constructor
TLB::TLB(){
    cacheHits = 0;
    cacheMisses = 0;
    virtualTime = 0;
}
/**
 * @brief Inserts VPN->PFN entries into hash table. If hash table is full, remove
 *        least recently used VPN from hash table before inserting new entry.
 * 
 * @param VPN: Virtual Page Number
 * @param PFN: Physical Frame Number
 */
void TLB::insertIntoTLB(uint32_t VPN, uint32_t PFN){
    if(maxCapacity > 0){
        // if cache at max capacity, remove least recently used address
        if(VPN2PFN.size() >= maxCapacity){
            uint32_t key_to_remove;
            //LRU_10.erase(LRU_10.begin());
            for(int i = 0; i < LRU_10.size(); i++){
                // check if least recently used address in hash map
                if(VPN2PFN.count(LRU_10[i]) > 0){
                    key_to_remove = LRU_10[i];
                    // remove entry from LRU 
                    LRU_10.erase(LRU_10.begin()+i);
                    // exit loop after finding VPN to remove from TLB
                    break; 
                }
            }
            // remove entry from TLB
            VPN2PFN.erase(key_to_remove);
        }
        // insert new VPN -> PFN mapping into hash table
        VPN2PFN[VPN] = PFN;
    }
}
/**
 * @brief Monitors array of recently used virtual page numbers. Most recently
 *        used VPNs are at the end of the array and least recently used are at the
 *        front of the array. Adjusts array size to always be less than 10. 
 * 
 * @param VPN: Virtual Page Number
 */
void TLB::RecentlyUsed(uint32_t VPN){
    // check if VPN already in recently accessed list
    for(int i = 0; i < LRU_10.size(); i++){
        // delete entry of VPN if found
        if(LRU_10[i] == VPN){
            // delete entry at given index
            LRU_10.erase(LRU_10.begin() + i);
            //break;  // exit loop
        }
    }
    // insert most recently used address to end of the list
    LRU_10.push_back(VPN);
    // check if size is greater than 10, delete the least recently used address
    if(LRU_10.size() > 10){
        LRU_10.erase(LRU_10.erase(LRU_10.begin()));
    }
}

// void TLB::TLB_insertion(uint32_t VPN, uint32_t PFN){
//     // check if tlb capacity defined (if we use it or not)
//     if(maxCapacity > 0){
//         VPN_to_PFN[VPN] = PFN;
//         // check if tlb full
//         if(VPN_to_PFN.size() > maxCapacity){
//             map<uint32_t, uint32_t>::iterator it = VPN_to_PFN.begin();
//             int minTime = VPN_virtualTime[it->first];
//             uint32_t keyToRemove = it->first;
//             for(++it; it!=VPN_to_PFN.end(); it++){
//                 if(minTime >= VPN_virtualTime[it->first]){
//                     keyToRemove = it->first;
//                     minTime = VPN_virtualTime[it->first];
//                 }
//             }
//             // find min time in VPN_virtualTime hash table
//             // for(it = VPN_virtualTime.begin(); it != VPN_virtualTime.end(); it++){
//             //     // check if VPN in TLB cache
//             //     if(VPN_to_PFN.count(it->first)){
//             //         // insert into array of tlb keys
//             //         keysInTLB.push_back(it->first);
//             //     }
//             // }

//             // remove from TLB cache
//             //printf("removing: %X\n", keyToRemove);
//             VPN_to_PFN.erase(keyToRemove);
//             // remove from recently used hash table
//             VPN_virtualTime.erase(keyToRemove);
//         }
//         // else insert mapping into TLB
//     }
// }
// void TLB::LRU_handler(uint32_t VPN){
//     // insert new entry or update value
//     VPN_virtualTime[VPN] = virtualTime;
//     // check if recently used hash table size greater than or equal 10
//     if(VPN_virtualTime.size() >= 10){
//         // remove least recently used from cache
//         map<uint32_t, int>::iterator it = VPN_virtualTime.begin();
//         uint32_t keyToRemove = it->first;
//         int minTime = it->second;
//         for(++it; it != VPN_virtualTime.end(); it++){
//             if(minTime >= it->second){
//                 keyToRemove = it->first;
//                 minTime = it->second;
//             }
//         }
//         // remove from list
//         VPN_virtualTime.erase(keyToRemove);
//     }
// }