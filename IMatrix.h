//
//  IMatrix.h
//  DCTcompressor
//
//  Created by Alejandro Alvarado on 24/09/14.
//  Copyright (c) 2014 Alejandro. All rights reserved.
//

#ifndef DCTcompressor_IMatrix_h
#define DCTcompressor_IMatrix_h

template <class T> class IMatrix
{
public:    
    virtual ~IMatrix() {};
    virtual void setValue(int row, int col, T val) = 0;
};

#endif
