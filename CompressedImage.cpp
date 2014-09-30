//
//  CompressedImage.cpp
//  DCTcompressor
//
//  Created by Alejandro Alvarado on 23/09/14.
//  Copyright (c) 2014 Alejandro. All rights reserved.
//

#include "bmp.h"
#include "CompressedImage.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

CompressedImage::CompressedImage(std::string path)
{
    ifstream file(path);
    vector<char> buffer;
    if(file)
    {
        file.seekg(0, ios::end); //Mover posición al final del buffer
        int length = file.tellg(); //Obtener posición en el buffer
        file.seekg(0, ios::beg); //Regresar el buffer
        buffer.resize(length); //dar tamaño al buffer
        file.read(&buffer[0],length); //escribir en el buffer
        file_header = (PBITMAPFILEHEADER)(&buffer[0]); //castear buffer al header de imagen
        info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));
        this->width = info_header->biWidth;
        this->height = info_header->biHeight;
        color_data = vector<char>(buffer.begin() + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER), buffer.end());
        color_data.shrink_to_fit();
        file.close();
    }
    
    
}

CompressedImage::~CompressedImage()
{
   
}

void CompressedImage::setValue(int row, int col, char val)
{
    color_data[row * width + col] = val;
}

char CompressedImage::getValue(int row, int col)
{
    return  color_data[row * width + col];
}

