//
//  Demo.cpp
//  DCTcompressor
//
//  Created by Alejandro Alvarado on 14/10/14.
//  Copyright (c) 2014 Alejandro. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "stdio.h"
#include "DCTCompressor.h"
#include <vector>
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


int main()
{
    DCTCompressor* compressor = new DCTCompressor();
    //matriz a comprimir
    vector< vector < double > > test_mat =
    {
        {15, 127, 127, 127, 87, 97, 27, 127},
        {127, 127, 127, 127, 17, 127, 127, 127},
        {127, 20, 127, 127, 77, 127, 127, 127},
        {127, 127, 127, 127, 47, 127, 127, 127},
        {127, 127, 127, 127, 57, 127, 127, 127},
        {127, 127, 127, 127, 17, 127, 127, 127},
        {127, 127, 127, 127, 127, 127, 127, 127},
        {127, 127, 127, 127, 127, 127, 127, 127}
    };
    
    print_mat(test_mat, 'f');
    
    vector< vector < double > > dct_t = compressor->transpose(DCTCompressor::dct_mat);
    vector<vector<double>> prod = compressor->mult_square_mat(DCTCompressor::dct_mat, test_mat);
}