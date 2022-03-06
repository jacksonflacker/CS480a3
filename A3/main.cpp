#include<iostream>
#include <unistd.h>

using namespace std;

bool processCommandLineArguments(int, char**);

int main(int argc, char **argv){
    if(!processCommandLineArguments(argc, argv)){
        exit(EXIT_FAILURE);
    }
    return 0;
}

bool processCommandLineArguments(int argc, char **argv){
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
    cout << "File found:\t"<<argv[optind++]<<endl;
    int level = 0;
    for(int i = optind; i < argc; i++){
        int level_bits = atoi(argv[i]);
        if(level_bits <= 0){
            cout << "Level "<< level <<" page table must be at least 1 bit\n";
            return false;
        }
        cout << "Page Level " << level++ << " has "<<level_bits<<" bytes.\n";
        total_level_bits += level_bits;
    }
    if(total_level_bits > 28){
        cout << "Too many bits used in page tables\n";
        return false;
    }
    return true;
}