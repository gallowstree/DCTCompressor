make: DCTcompressor

DCTcompressor: 
	g++ IMatrix.h CompressedImage.cpp main.cpp -std=c++11

clean:
	rm *.out
