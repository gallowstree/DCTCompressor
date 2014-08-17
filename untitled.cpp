#include "stdio.h"
#include <fstream>
#include <vector>
#include <iostream>
#include "bmp.h"
#include "ImageMatrix.h"
using namespace std;

PBITMAPFILEHEADER file_header;
PBITMAPINFOHEADER info_header;

short aux[8][8];


void fill_aux(int row_start, int col_start, ImageMatrix* img)
{	
	int col_start_b = col_start;
	for(int row = 0; row < 8; row++, row_start++)
	{
		for(int col = 0; col < 8; col++, col_start++)
		{
			aux[row][col] = img->getValue(row_start, col_start);		
		}
		col_start = col_start_b;		
	}
}

int main()
{
	ImageMatrix* img = new ImageMatrix("/Users/alejandroalvarado/Desktop/lena512.bmp");	
	cout << img->getValue(16,2) << endl;
    fill_aux(16,0,img);		
    cout << aux[0][2] << endl;
	return 0;
}

