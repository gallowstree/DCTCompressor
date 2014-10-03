//
//  DCTCompressor.h
//  DCTcompressor
//
//  Created by Alejandro Alvarado on 3/10/14.
//  Copyright (c) 2014 Alejandro. All rights reserved.
//

#ifndef __DCTcompressor__DCTCompressor__
#define __DCTcompressor__DCTCompressor__

#include <stdio.h>
#include <vector>
#include "ImageMatrix.h"
#include "CompressedImage.h"
#include "IMatrix.h"
#include "math.h"
#include <fstream>

class DCTCompressor
{
private:
    vector< vector<double> > aux;
    const int sub_matrix_size = 8;
    vector< vector < double > > dct_mat =
    {
        {0.3536,  0.3536,  0.3536,  0.3536,  0.3536,  0.3536,  0.3536,  0.3536},
        {0.4904,  0.4157,  0.2778,  0.0975, -0.0975, -0.2778, -0.4157, -0.4904},
        {0.4619,  0.1913, -0.1913, -0.4619, -0.4619, -0.1913,  0.1913 , 0.4619},
        {0.4157, -0.0975, -0.4904, -0.2778,  0.2778,  0.4904,  0.0975, -0.4157},
        {0.3536, -0.3536, -0.3536,  0.3536,  0.3536, -0.3536, -0.3536,  0.3536},
        {0.2778, -0.4904,  0.0975,  0.4157, -0.4157, -0.0975,  0.4904, -0.2778},
        {0.1913, -0.4619,  0.4619, -0.1913, -0.1913,  0.4619, -0.4619,  0.1913},
        {0.0975, -0.2778,  0.4157, -0.4904,  0.4904, -0.4157,  0.2778 ,-0.0975}
    };
    vector<vector<short>> quantiztion_50 =
    {
        {16,  11,  10,  16,  24,  40,  51,  61},
        {12,  12,  14,  19,  26,  58,  60,  55},
        {14,  13,  16,  24,  40,  57,  69,  56},
        {14,  17,  22,  29,  51,  87,  80,  62},
        {18,  22,  37,  56,  68, 109, 103,  77},
        {24,  35,  55,  64,  81, 104, 113,  92},
        {49,  64,  78,  87, 103, 121, 120,  101},
        {72,  92,  95,  98, 112, 100, 103,  99}
    };

    
public:
    void fill_aux(int row_start, int col_start, IMatrix<char>* img);

    /*MÉTODOS DE OPERACIONES CON MATRICES (HAY QUE MOVERLOS A OTRA CLASE)*/
    
    template<class T>
    void init_square_mat(int size, vector<vector<T>> &mat);
    template<class T, class T2>
    vector<vector<double>> mult_square_mat(vector<vector<T>> &mat1, vector<vector<T2>> &mat2);
    template<class T, class T2>
    vector<vector<char>> mult_square_mat_char(vector<vector<T>> &mat1, vector<vector<T2>> &mat2);
    template<class T>
    vector<vector<double>> transpose(vector<vector<T>> &mat);
    
    /*CUANTIZACIÓN*/
    
    vector<vector<short>> get_quantization_matrix(int quality);
    template<class T>
    vector<vector<short>> quantize(vector<vector<T>> &mat, bool inverse);
    template<class T>
    
    /*RUN LENGTH ENCODE*/
    
    vector<char> run_length_encode(vector<T> &vec);
    template <class T>
    vector<T> run_length_decode(vector<T> in);
    
    /*ZIG ZAG*/
    
    template<class T>
    vector<short> zig_zag_matrix(vector<vector<T>> &mat);
    template<class T>
    vector<vector<T>> unzig_zag_matrix(vector<T> &vec, int dimension);

    /*COMPRESIÓN*/
    
    int dct_compress(ImageMatrix *img, ofstream &file);
    
    /*DESCOMPRESIÓN*/
    
    void dct_decompress(CompressedImage* img, ofstream &file);
    
};





#endif /* defined(__DCTcompressor__DCTCompressor__) */
