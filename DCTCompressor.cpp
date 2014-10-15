//
//  DCTCompressor.cpp
//  DCTcompressor
//
//  Created by Alejandro Alvarado on 3/10/14.
//  Copyright (c) 2014 Alejandro. All rights reserved.
//

#include "DCTCompressor.h"
using namespace std;

std::vector< std::vector< double > > DCTCompressor::dct_mat =
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

std::vector<std::vector<short>> DCTCompressor::quantiztion_50 =
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

void DCTCompressor::compress_image(string path, string out, int quality)
{
    ImageMatrix* img = new ImageMatrix(path);
    ofstream file(out, ios::binary);
    file.write(reinterpret_cast<const char*>(img->file_header), sizeof(BITMAPFILEHEADER));
    file.write(reinterpret_cast<const char*>(img->info_header), sizeof(BITMAPINFOHEADER));
    
    int compressed_bytes = dct_compress(img, file);
    
    file.close();
    delete img;
    
    /*cout << "Se leyeron " << img->size << " bytes en la imagen original."<<endl;
    cout << "Se escribieron " << compressed_bytes <<" bytes que representan los datos de la imagen comprimida." <<endl;
    cout << "Y " << sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) << " de headers del archivo" <<endl;*/
    
}


int DCTCompressor::dct_compress(ImageMatrix *img, ofstream &file)
{
    vector< vector < double > > dct_t = transpose(dct_mat);
    init_square_mat(sub_matrix_size, aux);
    vector<vector<double>> prod;
    vector<vector<short>> q;
    int total_size = 0;
    for(int row = 0; row< (int)(img->width); row+=sub_matrix_size)
    {
        for(int col = 0; col < (int)(img->width); col+=sub_matrix_size)
        {
            fill_aux(row, col, img);
            prod = mult_square_mat(dct_mat, aux);
            prod = mult_square_mat(prod, dct_t);
            q = quantize(prod, false);
            vector<short> flat = zig_zag_matrix(q);
            vector<char> writeMe = run_length_encode(flat);
            char size = writeMe.size();
            file.write((const char *)&writeMe[0], size);
            total_size+= size;
        }
    }
    return total_size;
}


template<class T>
vector<char> DCTCompressor::run_length_encode(vector<T> &vec)
{
    vector<char> result;
    char zero_count = 0;
    for(int i = 0; i < vec.size(); i++)
    {
        if(vec[i] != 0)
        {
            if(zero_count != 0)
            {
                result.push_back(0);
                result.push_back(zero_count);
                zero_count = 0;
            }
            result.push_back((char)vec[i]);
        }
        else
        {
            zero_count++;
        }
        
    }
    if(zero_count != 0)
    {
        result.push_back(0);
        result.push_back(zero_count);
    }
    
    return result;
}


template<class T>
vector<short> DCTCompressor::zig_zag_matrix(vector<vector<T>> &mat)
{
    vector<T> res;
    int row = 0, col = 0, i = 0;
    
    while(row < mat.size())
    {
        res.push_back(mat[row][col]);
        
        if(row == mat.size() - 1)
        {
            row = col + 1;
            col = mat.size()- 1;
        }
        else if(col == 0)
        {
            col = row + 1;
            row = 0;
        }
        else
        {
            row++;
            col--;
        }
    }
    return res;
}


void DCTCompressor::decompress_image(string path, string out, int quality)
{
    ofstream file(out, ios::binary);
    CompressedImage* c = new CompressedImage(path);
    
    file.write(reinterpret_cast<const char*>(c->file_header), sizeof(BITMAPFILEHEADER));
    file.write(reinterpret_cast<const char*>(c->info_header), sizeof(BITMAPINFOHEADER));
    write_grayscale_pallette(file);
    
    dct_decompress(c, file);
    file.close();
    delete c;
    
}

void DCTCompressor::write_grayscale_pallette(ofstream &file)
{
    //generar la paleta
    for (int i = 0; i < 128; i++)
    {
        file << (char)i;
        file << (char)i;
        file << (char)i;
        file << (char)0;
    }
    
    for (int i = -128; i < 0; i++)
    {
        file << (char)i;
        file << (char)i;
        file << (char)i;
        file << (char)0;
    }

}

void DCTCompressor::dct_decompress(CompressedImage* img, ofstream &file)
{
    img->color_data = run_length_decode(img->color_data);
    
    vector< vector < double > > dct_t = transpose(dct_mat);
    init_square_mat(sub_matrix_size, aux);
    vector<vector<double>> prod;
    vector<vector<short>> q;
    vector<vector<char>> current;
    vector<char> buffer(img->width * img->height);
    
    int col_start = 0, row_start = 0, j=0;
    
    for(int i = 0; i < img->color_data.size(); i+= sub_matrix_size*sub_matrix_size)
    {
        vector<char> sub_vector(img->color_data.begin() + i, img->color_data.begin() + i + sub_matrix_size*sub_matrix_size);
        current = unzig_zag_matrix(sub_vector, sub_matrix_size);
        q = quantize(current, true);
        prod = mult_square_mat(dct_t, q);
        current = mult_square_mat_char(prod, dct_mat);
        
        
        for (int row = 0; row < sub_matrix_size; row++, row_start++)
        {
            for (int col = 0; col < sub_matrix_size; col++, col_start++)
            {
                buffer[row_start * img->width + col_start] = current[row][col];
            }
            col_start = j;
        }
        if(j == img->width -sub_matrix_size)
        {
            col_start = j = 0;
        }
        else if(col_start < img->width-1)
        {
            row_start -= sub_matrix_size;
            col_start+=sub_matrix_size;
            j+=sub_matrix_size;
        }
        
    }
    file.write((const char *)&buffer[0], buffer.size());
    
}


template <class T>
vector<T> DCTCompressor::run_length_decode(vector<T> in)
{
    vector<T> out;
    for(int i = 0; i < in.size(); i++)
    {
        if(in[i] == 0)
        {
            for(int j = 0; j < in[i+1]; j++)
                out.push_back(0);
            i++;
        }
        else
        {
            out.push_back(in[i]);
        }
    }
    return out;
}

template<class T>
vector<vector<T>> DCTCompressor::unzig_zag_matrix(vector<T> &vec, int dimension)
{
    vector<vector<T>> mat(dimension*dimension);
    init_square_mat(dimension*dimension, mat);
    int row = 0, col = 0, i = 0;
    
    while(row < dimension)
    {
        mat[row][col] = vec[i++];
        if(row == dimension - 1)
        {
            row = col + 1;
            col = dimension - 1;
        }
        else if(col == 0)
        {
            col = row + 1;
            row = 0;
        }
        else
        {
            row++;
            col--;
        }
    }
    
    return mat;
}



void DCTCompressor::fill_aux(int row_start, int col_start, IMatrix<char>* img)
{
    int col_start_b = col_start;
    for(int row = 0; row < sub_matrix_size; row++, row_start++)
    {
        for(int col = 0; col < sub_matrix_size; col++, col_start++)
        {
            aux[row][col] = img->getValue(row_start, col_start);
        }
        col_start = col_start_b;
    }
}

template<class T>
void DCTCompressor::init_square_mat(int size, vector<vector<T>> &mat)
{
    mat.reserve(size);
    for ( int i = 0 ; i < size ; i++ )
        mat[i].resize(size);
}

template<class T, class T2>
vector<vector<double>> DCTCompressor::mult_square_mat(vector<vector<T>> &mat1, vector<vector<T2>> &mat2)
{
    int size = mat1.size();
    vector<vector<double>> product(size);
    init_square_mat(size, product);
    for(int row = 0; row < size; row++)
    {
        for(int r_col = 0; r_col < size; r_col++)
        {
            for(int col = 0; col < size; col++)
            {
                product[row][r_col] +=  mat1[row][col]*mat2[col][r_col];
            }
        }
    }
    return product;
}

template<class T, class T2>
vector<vector<char>> DCTCompressor::mult_square_mat_char(vector<vector<T>> &mat1, vector<vector<T2>> &mat2)
{
    int size = mat1.size();
    vector<vector<char>> product(size);
    init_square_mat(size, product);
    for(int row = 0; row < size; row++)
    {
        for(int r_col = 0; r_col < size; r_col++)
        {
            for(int col = 0; col < size; col++)
            {
                product[row][r_col] +=  (char)(round(mat1[row][col]*mat2[col][r_col]));
            }
        }
    }
    return product;
}

template<class T>
vector<vector<double>> DCTCompressor::transpose(vector<vector<T>> &mat)
{
    int r_size = mat.size();
    int c_size = mat[0].size();
    vector<vector<double>> outtrans(c_size, vector<double>(r_size));
    for (int i = 0; i < r_size; ++i)
        for (int j = 0; j < c_size; ++j)
            outtrans[j][i] = mat[i][j];
    return outtrans;
}

DCTCompressor::DCTCompressor()
{
    
}

DCTCompressor::~DCTCompressor()
{
    //Implement me
}

vector<vector<short>> DCTCompressor::get_quantization_matrix(int quality)
{
    return quantiztion_50; //TODO: generar matrices dependiendo de la calidad
}

template<class T>
vector<vector<short>> DCTCompressor::quantize(vector<vector<T>> &mat, bool inverse)
{
    vector<vector<short>> res(sub_matrix_size);
    init_square_mat(sub_matrix_size,res);
    vector<vector<short>> q = get_quantization_matrix(50);
    for(int r = 0; r < sub_matrix_size; r++)
        for(int c = 0; c < sub_matrix_size; c++)
        {
            res[r][c] = round(inverse ? mat[r][c]*q[r][c] : mat[r][c]/q[r][c]);
        }
    
    return res;
}
