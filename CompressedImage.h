//
//  CompressedImage.h
//  DCTcompressor
//
//  Created by Alejandro Alvarado on 23/09/14.
//  Copyright (c) 2014 Alejandro. All rights reserved.
//

#ifndef __DCTcompressor__CompressedImage__
#define __DCTcompressor__CompressedImage__


#include <vector>
#include "IMatrix.h"

class CompressedImage : public IMatrix<char>
{
    private:
        
    public:
        int width, height;
        PBITMAPFILEHEADER file_header;
        PBITMAPINFOHEADER info_header;
        std::vector<char> color_data;
    CompressedImage(std::string path);
        ~CompressedImage();
        void setValue(int row, int col, char val);
        char getValue(int row, int col);

    
    
};
#endif /* defined(__DCTcompressor__CompressedImage__) */
