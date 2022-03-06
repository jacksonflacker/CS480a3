#include<iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include "PageTable.h"

using namespace std;

bool processCommandLineArguments(int, char**, PageTable&, FILE*);

int main(int argc, char **argv){
    PageTable pgTable;
    FILE *fp;
    if(!processCommandLineArguments(argc, argv, pgTable, fp)){
        exit(EXIT_FAILURE);
    }
    pgTable.
    fclose(fp);
    return 0;
}

bool processCommandLineArguments(int argc, char **argv, PageTable &pgTable, FILE *fp){
    int total_level_bits = 0;
    if(argc <= 1){return false;}
    int option;
    while((option = getopt(argc, argv, "n:c:o:")) != -1){
        switch(option){
            case 'n':
                if(optarg){
                    cout << "Process only the first "<< optarg <<" memory accesses/references.\n";
                }
                else{
                    cout << "Processes all addresses if not present.\n";
                }
                break;
            case 'c':
                cout << "Cache capacity:\t"<<optarg<<endl;
                break;
            case 'o':
                cout << "Output mode:\t"<<optarg<<endl;
            default:
                break;
        }
    }
    // no page level bits included
    if(optind + 1 == argc){
        cout << "Must provide Page Level Bits as argument\n";
        return false;
    }
    // open file
    fp = fopen(argv[optind], "r");
    // return false if file unable to open
    if (fp == NULL){
        cout <<"Unable to open file <<"<<argv[optind]<<">>.\n";
        return false;
    }
    int level = 0;
    for(int i = optind+1; i < argc; i++){
        int level_bits = atoi(argv[i]);
        if(level_bits <= 0){
            cout << "Level "<< level <<" page table must be at least 1 bit\n";
            return false;
        }
        pgTable.EntryCount.push_back(pow(2, level_bits));
        level++;
        // cout << "Page Level " << level++ << " has "<<level_bits<<" bytes.\n";
        // total_level_bits += level_bits;
    }
    if(total_level_bits > 28){
        cout << "Too many bits used in page tables\n";
        return false;
    }
    pgTable.offset = 32 - total_level_bits;
    pgTable.LevelCount = level;
    return true;
}