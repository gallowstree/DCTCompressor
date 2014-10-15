make: DCTcompressor

DCTcompressor: 
	g++  CompressedImage.cpp ImageMatrix.cpp DCTCompressor.cpp main.cpp -std=c++11 -o DctCompressor.out
Demo:
    g++  CompressedImage.cpp ImageMatrix.cpp DCTCompressor.cpp Demo.cpp -std=c++11 -o Demo.out

clean:
	rm *.out
