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
using namespace std;
class CompressedImage
{
    private:
        
    public:
        int width, height;
        PBITMAPFILEHEADER file_header;
        PBITMAPINFOHEADER info_header;
        vector<char> color_data;
        CompressedImage(std::string path);
        ~CompressedImage();
        
    
};
#endif /* defined(__DCTcompressor__CompressedImage__) */
