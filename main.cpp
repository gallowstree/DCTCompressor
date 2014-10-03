
#include <iostream>
#include <sys/time.h>
#include "stdio.h"
#include "DCTCompressor.h"


using namespace std;

template<class T>
void print_mat(vector<vector<T>> mat, char format)
{
    for(int r = 0; r < DCTCompressor::sub_matrix_size; r++){
        for(int c = 0; c <  DCTCompressor::sub_matrix_size; c++)
        {
            if(format == 'f')
                printf("%7.3f ", mat[r][c]);
            else if(format == 'i')
                printf("%3d ", mat[r][c]);
        }
        cout <<endl;
    }
    cout <<endl<<endl<<endl;
}

template<class T>
void print_v(vector<T> &vec)
{
    for(int i = 0; i < vec.size(); i++)
        cout << (int)vec[i] << " ";
    cout << endl << endl;
}


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

