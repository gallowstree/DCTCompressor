#include "stdio.h"
#include "ImageMatrix.h"
#define DEBUG ;
using namespace std;

vector< vector<short> > aux;
int sub_matrix_size;


void fill_aux(int row_start, int col_start, ImageMatrix* img)
{	
	cout << "\n------------" << endl;
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
	ImageMatrix* img = new ImageMatrix("/Users/alejandroalvarado/Projects/DCTcompressor/Images/lena512.bmp");	

	/*cout << img->getValue(16,2) << endl; //prueba del método fill_aux
    fill_aux(16,0,img);		
    cout << aux[0][2] << endl;*/
    
	for(int row = 0; row< (int)(img->width/*/sub_matrix_size*/); row+=sub_matrix_size)
	{
		for(int col = 0; col < (int)(img->width/*/sub_matrix_size*/); col+=sub_matrix_size)
		{
			//cout << row << " " << col <<endl;
			fill_aux(row, col, img);
			
		}
	}

	return 0;
}

