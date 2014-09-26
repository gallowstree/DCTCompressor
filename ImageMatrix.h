//
//  ImageMatrix.h
//  DCTcompressor
//
//  Created by Alejandro Alvarado on 25/09/14.
//  Copyright (c) 2014 Alejandro. All rights reserved.
//

#ifndef DCTcompressor_ImageMatrix_h
#define DCTcompressor_ImageMatrix_h
#include <string>
#include "bmp.h"
#include "IMatrix.h"
class ImageMatrix : public IMatrix<char>
{
private:
    
public:
    int width, height, size;
    PBITMAPFILEHEADER file_header;
    PBITMAPINFOHEADER info_header;
    char * matrix;
    ImageMatrix(std::string path);
    ~ImageMatrix();
    void setValue(int row, int col, char val);
    char getValue(int row, int col);
};


#endif
