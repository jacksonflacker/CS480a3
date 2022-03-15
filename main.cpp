#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <numeric>
#include <stdint.h>
#include <map>
#include "TLB.h"
#include "PageTable.h"
#include "Level.h"
#include "tracereader.h"
#include "output_mode_helpers.h"
#include "Map.h"

using namespace std;

typedef struct{
    long processes;
    int cache_cap;
    string output_mode;
    const char *filePath;
}CMD;

bool ProcessCommandLineArguments(int, char**, PageTable*, CMD*);
void ProcessBitmaskAry(int, PageTable*, int, int);
void pageInsert(PageTable* , unsigned int , unsigned int&);
unsigned int returnOffset(PageTable*, unsigned int);
unsigned int returnVirtualPageNumber(PageTable*, unsigned int);

int main(int argc, char **argv){
    // pointer to page table object
    PageTable *pgTable = new PageTable();
    // pointer to map object
    Map* mapPointer;
    // pointer to TLB object
    TLB *tlbCache;

    FILE *ifp;	        /* trace file */
    p2AddrTr trace;	/* traced address */
    // frame number
    unsigned int frame = 0;
    // virtual page number
    unsigned int virtualPageNumber;
    // flag for cache hits
    bool tlbHit = false;
    // flag for page hits
    bool pageHit = false;
    unsigned long i = 0;  /* instructions processed */
    // pointer to CMD object
    CMD *args = new CMD;

    // process command line arguments and store information
    if(!ProcessCommandLineArguments(argc,argv,pgTable,args)){
        delete(pgTable);
        delete(args);
        exit(1);
    }
    // allocates level 0. sets depth to 0 and sizes array to entry count
    pgTable->AllocateFirstLevel(pgTable);

    // exit after printing bit masks, do need to open file
    if(args->output_mode == "bitmasks"){
        report_bitmasks(pgTable->levelCount, &(pgTable->BitmaskAry[0]));
        delete(pgTable);
        delete(args);
        exit(1);
    }
    // exit if file could not be opened
    if ((ifp = fopen(args->filePath,"rb")) == NULL) {
        cout << "Unable to open <<"<<args->filePath<<">>\n";
        delete(pgTable);
        delete(args);
        exit(1);
    }
    // pointer to TLB object
    tlbCache = new TLB();
    //store cache capacity, zero if none specified
    tlbCache->maxCapacity = args->cache_cap;
    // read trace file
    while (!feof(ifp) && i != args->processes) {
        /* get next address and process */
        if (NextAddress(ifp, &trace)) {
            i++;
            // print the offset of virtual address for N number of processes
            if(args->output_mode == "offset"){
                // return offset of virtual addres
                hexnum(returnOffset(pgTable, trace.addr));
            }
            else{
                // if cache is used
                if(args->cache_cap > 0){
                    // check if VPN mapped to frame in TLB
                    virtualPageNumber = returnVirtualPageNumber(pgTable, trace.addr);
                    // VPN is in hashtable if count is greater than 0
                    if(tlbCache->VPN2PFN.count(virtualPageNumber)){
                        // increment hit count
                        tlbCache->cacheHits++;
                        // set flag to true
                        tlbHit = true;
                        // insert into LRU queue
                        tlbCache->LastRecentlyUsed(virtualPageNumber);
                    }
                }
                // cache miss or cache not used, walk table
                if(!tlbHit){
                    // increment cache miss count
                    tlbCache->cacheMisses++;
                    tlbHit = false;
                    // returns pointer to map object or null
                    mapPointer = mapPointer->pageLookUp(pgTable,trace.addr);
                    // call page insert if map pointer is null
                    if(!mapPointer){
                        // page walk miss
                        pageHit = false;                       
                        // walk the table
                        pageInsert(pgTable,trace.addr, frame);
                        // insert virtual page number into hash table and
                        tlbCache->insertIntoTLB(virtualPageNumber, frame-1); 
                    }
                    // map pointer not null
                    else{
                        // page table hit
                        pageHit = true;
                        tlbCache->insertIntoTLB(virtualPageNumber, mapPointer->PFN);
                    }
                }
                int currFrame;
                // tlb hit
                if(tlbHit){
                    // currFrame = frame # in TLB cache
                    currFrame = tlbCache->VPN2PFN[virtualPageNumber];
                }
                // tlb miss, page hit
                else if(pageHit){
                    // currFrame = frame # of page hit
                    currFrame = mapPointer->PFN;
                }
                // page miss and tlb miss
                else{
                    // currFrame = frame # recently inserted
                    currFrame = frame-1;
                }
                // get offset of virtual address
                unsigned int dest = returnOffset(pgTable, trace.addr);
                // shift frame # by offset before concatenating w/ offset
                dest = (currFrame << pgTable->offset) + dest;
                // dest = physical address

                if(args->output_mode == "virtual2physical"){
                    report_virtual2physical(trace.addr,dest);
                }
                else if(args->output_mode == "vpn2pfn"){
                    if(!pageHit){
                        report_pagemap(pgTable->levelCount,&pgTable->currVPN[0], currFrame);
                    }
                    // tlb hit, or page miss
                    else{
                        // vector of multi level VPNs
                        vector<uint32_t> pages;
                        for(int j = 0; j < pgTable->levelCount; j++){
                            uint32_t temp = trace.addr;
                            temp = temp & pgTable->BitmaskAry[j];
                            temp = temp >> pgTable->ShiftAry[j];
                            pages.push_back(temp);
                        }
                        report_pagemap(pgTable->levelCount,&pages[0], currFrame);                     
                    }
                }
                else if(args->output_mode == "v2p_tlb_pt"){
                    report_v2pUsingTLB_PTwalk(trace.addr, dest, tlbHit, pageHit);
                    for(map<uint32_t,uint32_t>::iterator it = tlbCache->VPN2PFN.begin(); it != tlbCache->VPN2PFN.end(); it++){
                        printf("%x,\t", it->first);
                    }
                    cout << endl;
                }
            }
            // reset flags
            pageHit = false;
            tlbHit = false;
        }
    }
    // print summary
    if(args->output_mode == "" || args->output_mode == "summary"){
        report_summary(pow(2,pgTable->offset),
        tlbCache->cacheHits,
        pgTable->pageHits,
        i,
        pgTable->pageMisses,
        100);
    }
    /* clean up and return success */
    delete(tlbCache);
    delete(args);
    delete(pgTable);
    fclose(ifp);
    return 0;
}

unsigned int returnVirtualPageNumber(PageTable* pgTable, unsigned int virtualAddress){
    unsigned int VPN = (virtualAddress >> pgTable->offset);
    return (VPN << pgTable->offset);
}

bool ProcessCommandLineArguments(int argc, char **argv, PageTable* pgTable, CMD *args){
    // terminate program if no mandatory arguments present
    if(argc <= 1){return false;}
    // 32-bit
    int bits = 32;
    args->processes = -1; // default val, if negative process all addresses
    args->cache_cap = 0;
    args->output_mode = "";
    int total_level_bits = 0;   /* total bits amongst all pages */
    int option; /* getopt() option */
    //int cache_capacity = 0;
    int level = 0; /* number of page levels */

    //string output_mode = "";

    while((option = getopt(argc, argv, "n:c:o:")) != -1){
        switch(option){
            case 'n':
                args->processes = atoi(optarg);
                //cout << "Process only the first "<< optarg <<" memory accesses/references.\n";            
                break;
            case 'c':
                if(atoi(optarg) < 0){
                    cout << "Cache capacity must be a number, greater than or equal to 0\n";
                    return false;
                }
                args->cache_cap = atoi(optarg);
                //cout << "Cache capacity:\t"<<optarg<<endl;
                break;
            case 'o':
                args->output_mode = optarg;
                //cout << "Output mode:\t"<<optarg<<endl;
            default:
                break;
        }
    }
    cout << args->processes<<endl;
    // no page level bits included
    if(optind + 1 == argc){
        cout << "Must provide Page Level Bits as argument\n";
        return false;
    }
    //store filepath
    cout<<argv[optind]<<endl;
    args->filePath = argv[optind];
    // loop through size of pages
    for(int i = optind+1; i < argc; i++){
        cout<<argv[i]<<endl;
        int level_bits = atoi(argv[i]);
        // return false if no level bits provided
        if(level_bits < 1){
            cout <<"Level "<<level<<" page table must be at least 1 bit\n";
            return false;
        }
        // process bitmasking for each level
        ProcessBitmaskAry(bits, pgTable, level_bits, level);
        // bits change based on bits each level occupies
        bits -= level_bits;
        // bits to shift for each level
        pgTable->ShiftAry.push_back(bits);
        // store size of levels
        pgTable->SizeOfLevels.push_back(level_bits);
        // store entry count for each level
        pgTable->EntryCount.push_back(pow(2,level_bits));
        // increment level count
        level++;
        total_level_bits += level_bits;
    }
    // store offset of virtual address
    pgTable->offset = bits; // remaining bits after level bits occupied
    // store total number of levels into struct
    pgTable->levelCount = level;
    // check if total number of bits is less than 28
    if(total_level_bits > 28){
        cout << "Too many bits used in page tables\n";
        return false;
    }
    return true;
}
// calculates bit masking for each page based on page size
void ProcessBitmaskAry(int bitsToShift, PageTable* pgTable, int level_bits, int curLevel){
    uint32_t maskVal = 0;
    for(int i = 0; i < bitsToShift; i++){
        maskVal = maskVal << 1;
        maskVal = maskVal | 1;
    }
    // unwanted bits in the bitmask
    int remove = pow(2,(bitsToShift - level_bits))-1;
    // 0xFFFFFFFF ^ 0x00FFFFFF = 0xFF000000
    maskVal = maskVal ^ remove;
    pgTable->BitmaskAry.push_back(maskVal);
}
void pageInsert(PageTable* pageTable, unsigned int virtualAddress, unsigned int &frame){
    Level *currLevel = pageTable->RootLevelPtr;
    bool VPN_notFound = true;
    unsigned int fullVPN = 0;
    do{
        // masking and shifting to extract VPN from logical address
        //printf("virtual address: %x\n",virtualAddress);
        unsigned int VPN = virtualAddress & pageTable->BitmaskAry[currLevel->depth];
        VPN = VPN >> pageTable->ShiftAry[currLevel->depth];
        fullVPN += VPN;
        pageTable->currVPN[currLevel->depth] = VPN;
        //printf("vitrual page number: %x\n",VPN);
        // check if leaf node
        if(currLevel->depth+1 == pageTable->levelCount){
            //check if map object is null
            if(currLevel->MapPtr[VPN] == NULL){
                //insert map pointer
                currLevel->MapPtr[VPN] = new Map();
                // store full virtual page number into map
                currLevel->MapPtr[VPN]->VPN = fullVPN;
                // insert frame and increment frame number after insert
                currLevel->MapPtr[VPN]->PFN = frame++;
                currLevel->MapPtr[VPN]->valid = true;
                //cout << "mapped new frame\n";
            }
            //set flag to false
            VPN_notFound = false;
        }
        // current level is not leaf node
        else{
            //check if level pointer at current vpn is null
            if(currLevel->NextLevelPtr[VPN] == NULL){
                // insert a new level pointer
                currLevel->NextLevelPtr[VPN] = new Level();
                // initialize new level depth
                currLevel->NextLevelPtr[VPN]->depth = currLevel->depth+1;
                // new level points back to pagetable
                currLevel->NextLevelPtr[VPN]->PageTablePtr = pageTable;
                int currDepth = currLevel->NextLevelPtr[VPN]->depth;
                //check if new level is a leaf node
                if(currDepth+1 == pageTable->levelCount){
                    // resize map array
                    currLevel->NextLevelPtr[VPN]->MapPtr.resize(pageTable->EntryCount[currDepth]);                    
                }
                else{
                    // resize next level array
                    currLevel->NextLevelPtr[VPN]->NextLevelPtr.resize(pageTable->EntryCount[currDepth]);
                }
            }
            // iterate through next level
            currLevel = currLevel->NextLevelPtr[VPN];
        }

    }while(VPN_notFound);
}
unsigned int returnOffset(PageTable* pageTable, unsigned int virtualAddress){
    int shift = accumulate(pageTable->SizeOfLevels.begin(),pageTable->SizeOfLevels.end(),0);
    virtualAddress = virtualAddress << shift;
    return (virtualAddress >> shift);
}