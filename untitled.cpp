#include "stdio.h"
#include <fstream>
#include <vector>
#include <iostream>
#include "bmp.h"

using namespace std;

PBITMAPFILEHEADER file_header;
PBITMAPINFOHEADER info_header;

short aux[8][8];

int main()
{
	ifstream file("/Users/alejandroalvarado/Desktop/imagen.bmp");
	vector<char> buffer;
	if(file)
	{
		file.seekg(0, ios::end); //Mover posición al final del buffer
		int length = file.tellg(); //Obtener posición en el buffer
		file.seekg(0, ios::beg); //Regresar el buffer
		printf("length: %i\n", length);
		
		buffer.resize(length); //dar tamaño al buffer
        file.read(&buffer[0],length); //escribir en el buffer

        file_header = (PBITMAPFILEHEADER)(&buffer[0]);
        info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));

        cout <<  length - file_header->bfOffBits << endl;
    	cout << file_header->bfSize << endl;
	    cout << info_header->biWidth << " " << info_header->biHeight << endl;

	    short bm_matrix  [info_header->biHeight][info_header->biWidth] ; 
		puts("\n");
	    for(int row = info_header->biHeight - 1; row >= 0 ; row--)
	    {
	    	for(int col = info_header->biWidth - 1; col >= 0 ; col--)
		    {
		    	bm_matrix[row][col] = buffer[ file_header->bfOffBits + row * info_header->biWidth + col] & 0xFF; 		    			    	
		    	printf("%03i ", bm_matrix[row][col]);
		    }
		    puts("\n");
	    }

	    
	}
	
	return 0;
}