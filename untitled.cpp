#define _USE_MATH_DEFINES
#include "math.h"
#include "stdio.h"
#include "ImageMatrix.h"
#define DEBUG 
using namespace std;

vector< vector<short> > aux;
vector< vector < double > > dct_mat;
int sub_matrix_size;



void init_dct(int size)
{
	dct_mat.reserve(sub_matrix_size);
	for ( int row = 0 ; row < sub_matrix_size ; row++ )
	{
   		dct_mat[row].resize(sub_matrix_size);
		for(int col = 0; col <  sub_matrix_size; col++)
		{
			dct_mat[row][col] = row == 0 ?
								sqrt(1/8) : 0;
		}
	}
}

void fill_aux(int row_start, int col_start, ImageMatrix* img)
{		
	int col_start_b = col_start;
	for(int row = 0; row < sub_matrix_size; row++, row_start++)
	{
		cout << endl;
		for(int col = 0; col < sub_matrix_size; col++, col_start++)
		{
			aux[row][col] = img->getValue(row_start, col_start);	
			#ifdef DEBUG
			printf("%03i ", aux[row][col]);
			#endif
		}
		col_start = col_start_b;		
	}
}

void init_aux(int size)
{
	aux.reserve(sub_matrix_size);
	for ( int i = 0 ; i < sub_matrix_size ; i++ )
   		aux[i].resize(sub_matrix_size);
}



int main()
{
	sub_matrix_size = 8;
	init_aux(sub_matrix_size);
	ImageMatrix* img = new ImageMatrix("/Users/alejandroalvarado/Projects/DCTcompressor/Images/imagen.bmp");	
    
	for(int row = 0; row< (int)(img->width); row+=sub_matrix_size)
	{
		for(int col = 0; col < (int)(img->width); col+=sub_matrix_size)
		{			
			fill_aux(row, col, img);			
		}
	}

	return 0;
}

