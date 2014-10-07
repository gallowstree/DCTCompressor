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

typedef struct CompressionResult {
    int original_filesize;
    int original_image_data_size;
    int compressed_bytes;
    int header_bytes;
    double compression_ratio;
    
} CompressionResult;


class DCTCompressor
{
private:
    std::vector< std::vector<double> > aux;
    
    std::vector< std::vector< double > > dct_mat =
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
    std::vector<std::vector<short>> quantiztion_50 =
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
    static const int sub_matrix_size = 8;
    void fill_aux(int row_start, int col_start, IMatrix<char>* img);
    /*CONSTRUCTOR Y DESTRUCTOR*/
    
    DCTCompressor();
    ~DCTCompressor();
    
    /*MÉTODOS DE OPERACIONES CON MATRICES (HAY QUE MOVERLOS A OTRA CLASE)*/
    
    template<class T>
    void init_square_mat(int size, std::vector<std::vector<T>> &mat);
    template<class T, class T2>
    std::vector<std::vector<double>> mult_square_mat(std::vector<std::vector<T>> &mat1, std::vector<std::vector<T2>> &mat2);
    template<class T, class T2>
    std::vector<std::vector<char>> mult_square_mat_char(std::vector<std::vector<T>> &mat1, std::vector<std::vector<T2>> &mat2);
    template<class T>
    std::vector<std::vector<double>> transpose(std::vector<std::vector<T>> &mat);
    
    /*CUANTIZACIÓN*/
    
    std::vector<std::vector<short>> get_quantization_matrix(int quality);
    template<class T>
    std::vector<std::vector<short>> quantize(std::vector<std::vector<T>> &mat, bool inverse);
    template<class T>
    
    /*RUN LENGTH ENCODE*/
    
    std::vector<char> run_length_encode(std::vector<T> &vec);
    template <class T>
    std::vector<T> run_length_decode(std::vector<T> in);
    
    /*ZIG ZAG*/
    
    template<class T>
    std::vector<short> zig_zag_matrix(std::vector<std::vector<T>> &mat);
    template<class T>
    std::vector<std::vector<T>> unzig_zag_matrix(std::vector<T> &vec, int dimension);

    /*COMPRESIÓN*/
    
    int dct_compress(ImageMatrix *img, std::ofstream &file);
    
    /*DESCOMPRESIÓN*/
    
    void dct_decompress(CompressedImage* img, std::ofstream &file);
    void write_grayscale_pallette(std::ofstream &file);
    
    /*ENTRY POINTS*/
    
    void compress_image(std::string in, std::string out, int quality);
    void decompress_image(std::string in, std::string out, int quality);
    
};





#endif /* defined(__DCTcompressor__DCTCompressor__) */