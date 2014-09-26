make: DCTcompressor

DCTcompressor: 
	g++  CompressedImage.cpp ImageMatrix.cpp main.cpp -std=c++11

clean:
	rm *.out
