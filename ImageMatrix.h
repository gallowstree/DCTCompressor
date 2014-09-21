#include "bmp.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
class ImageMatrix
{
	private:
		
	public:
		int width, height;
        PBITMAPFILEHEADER file_header;
        PBITMAPINFOHEADER info_header;
        char * matrix;
		ImageMatrix(std::string path)
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
        		matrix = new char[width * height];
    		 	for(int row = 0; row < height ; row++)
			    {
			    	for(int col = 0; col < width ; col++)
				    {
				    	matrix[row * width + col] =/* row * width + col;*/buffer[ file_header->bfOffBits + row * width + col]; //& 0xFF; //Llenar matriz interna, volver positivo con el AND
				    }				    
			    }
			}
			
			
		}

		~ImageMatrix()
		{
			delete matrix;
		}

		void setValue(int row, int col, short val)
		{
			 matrix[row * width + col] = val; 
		}

		short getValue(int row, int col)
		{
			return  matrix[row * width + col];
		}


};