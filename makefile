make: DCTcompressor

DCTcompressor: 
	g++  CompressedImage.cpp ImageMatrix.cpp DCTCompressor.cpp main.cpp -std=c++11

clean:
	rm *.out
