
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


int main()
{
    //test();
    DCTCompressor* compressor = new DCTCompressor();
    compressor->compress_image("Images/lena512.bmp","compressed.bin",50);
    compressor->decompress_image("compressed.bin","reconstructed.bmp",50);
    delete compressor;
    return 0;
}

