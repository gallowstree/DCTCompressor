
#include <iostream>
#include <sys/time.h>
#include "stdio.h"
#include "DCTCompressor.h"


using namespace std;



double get_total_time()
{
    struct timeval time;
    gettimeofday(&time,NULL);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

double get_cpu_time()
{
    return (double)clock() / CLOCKS_PER_SEC;
}

void print_help(string name)
{
    cout << name << " -c path/to/input path/to/output quality "<<  endl;
    cout << name << " -d path/to/input path/to/output quality "<<  endl;
}

//1: -c(ompress)||-d(ecompress)
//2: input file
//3: output file
//4: quality
int main(int argc, char* argv[])
{
    //test();
    if(argc != 5)
    {
        print_help(argv[0]);
        return -1;
    }

    DCTCompressor* compressor = new DCTCompressor();
    
    if(strcmp(argv[1],"-c") == 0)
        compressor->compress_image(argv[2],argv[3], stoi( argv[4] ));
    else if(strcmp(argv[1],"-d") == 0)
        compressor->decompress_image(argv[2],argv[3], stoi( argv[4] ));
    else
        print_help(argv[0]);
    
    delete compressor;
    return 0;
}

