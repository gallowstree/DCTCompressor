#define _USE_MATH_DEFINES
#include "math.h"
#include "stdio.h"
#include "ImageMatrix.h"
#include "CompressedImage.h"
#include <sys/time.h>
//#include <
using namespace std;

//contendrá las secciones n*n de la imagen original
vector< vector<double> > aux;
//tamaño de las matrices en que se dividirá la imagen (para otros diferentes de 8, se debe recalcular la matriz dct)
const int sub_matrix_size = 8;
//Matriz dct de 8*8 precalculada, para mejorar performance.
vector< vector < double > > dct_mat =
{
    {0.3536,  0.3536,  0.3536,  0.3536,  0.3536,  0.3536,  0.3536,  0.3536},
    {0.4904,  0.4157,  0.2778,  0.0975, -0.0975, -0.2778, -0.4157, -0.4904},
    {0.4619,  0.1913, -0.1913, -0.4619, -0.4619, -0.1913,  0.1913 , 0.4619},
    {0.4157, -0.0975, -0.4904, -0.2778,  0.2778,  0.4904,  0.0975, -0.4157},
    {0.3536, -0.3536, -0.3536,  0.3536,  0.3536, -0.3536, -0.3536,  0.3536},
    {0.2778, -0.4904,  0.0975,  0.4157, -0.4157, -0.0975,  0.4904, -0.2778},
    {0.1913, -0.4619,  0.4619, -0.1913, -0.1913,  0.4619, -0.4619,  0.1913},
    {0.0975, -0.2778,  0.4157, -0.4904,  0.4904, -0.4157,  0.2778 ,-0.0975}
};
//Matriz de cuantización estándard, nivel de calidad 50/100.
vector<vector<short>> quantiztion_50 =
{
    {16,  11,  10,  16,  24,  40,  51,  61},
    {12,  12,  14,  19,  26,  58,  60,  55},
    {14,  13,  16,  24,  40,  57,  69,  56},
    {14,  17,  22,  29,  51,  87,  80,  62},
    {18,  22,  37,  56,  68, 109, 103,  77},
    {24,  35,  55,  64,  81, 104, 113,  92},
    {49,  64,  78,  87, 103, 121, 120,  101},
    {72,  92,  95,  98, 112, 100, 103,  99}
};

//Llenar la matriz auxiliar con la sección n*n de la imagen que empieza en row_start, col_start
void fill_aux(int row_start, int col_start, ImageMatrix* img)
{
    int col_start_b = col_start;
    for(int row = 0; row < sub_matrix_size; row++, row_start++)
    {
        for(int col = 0; col < sub_matrix_size; col++, col_start++)
        {
            aux[row][col] = img->getValue(row_start, col_start);
        }
        col_start = col_start_b;
    }
}

//Inicializa/Dimensiona una matriz
template<class T>
void init_square_mat(int size, vector<vector<T>> &mat)
{
    mat.reserve(size);
    for ( int i = 0 ; i < size ; i++ )
        mat[i].resize(size);
}

//Multiplica dos matrices cuadradas
template<class T, class T2>
vector<vector<double>> mult_square_mat(vector<vector<T>> &mat1, vector<vector<T2>> &mat2)
{
    int size = mat1.size();
    vector<vector<double>> product(size);
    init_square_mat(size, product);
    for(int row = 0; row < size; row++)
    {
        for(int r_col = 0; r_col < size; r_col++)
        {
            for(int col = 0; col < size; col++)
            {
                product[row][r_col] +=  mat1[row][col]*mat2[col][r_col];
            }
        }
    }
    return product;
}

//Transpone la matriz
template<class T>
vector<vector<double>> transpose(vector<vector<T>> &mat)
{
    int r_size = mat.size();
    int c_size = mat[0].size();
    vector<vector<double>> outtrans(c_size, vector<double>(r_size));
    for (int i = 0; i < r_size; ++i)
        for (int j = 0; j < c_size; ++j)
            outtrans[j][i] = mat[i][j];
    return outtrans;
}

//Genera una matriz de cuantización para la calidad indicada (calidad de 1 a 100)
//100 = máxima calidad, menor compresión.
vector<vector<short>> get_quantization_matrix(int quality)
{
    return quantiztion_50; //TO-DO: generar matrices dependiendo de la calidad
}

//Divide cada elemento de mat por la matriz de cuantización
//Si inverse es verdadero, multiplica en vez de dividir (para descompresión)
template<class T>
vector<vector<short>> quantize(vector<vector<T>> &mat, bool inverse)
{
    vector<vector<short>> res(sub_matrix_size);
    init_square_mat(sub_matrix_size,res);
    vector<vector<short>> q = get_quantization_matrix(50);
    for(int r = 0; r < sub_matrix_size; r++)
        for(int c = 0; c < sub_matrix_size; c++)
            res[r][c] = round(inverse ? mat[r][c]*q[r][c] : mat[r][c]/q[r][c]);
    
    return res;
}
template<class T>
void print_v(vector<T> &vec)
{
    for(int i = 0; i < vec.size(); i++)
        cout << (int)vec[i] << " ";
    cout << endl << endl;
}


vector<char> run_length_encode(vector<short> &vec)
{
    vector<char> result;
    char zero_count = 0;
    for(int i = 0; i < vec.size(); i++)
    {
        if(vec[i] != 0)
        {
            if(zero_count != 0)
            {
                result.push_back(0);
                result.push_back(zero_count);
                zero_count = 0;
            }
            result.push_back((char)vec[i]);
        }
        else
        {
            zero_count++;
        }

    }
    if(zero_count != 0)
    {
        result.push_back(0);
        result.push_back(zero_count);
    }
    //print_v(result);

    return result;
}


template<class T>
vector<short> zig_zag_matrix(vector<vector<T>> &mat)
{
    vector<T> res;
    int row = 0, col = 0;
    
    while(row < mat.size())
    {
        res.push_back(mat[row][col]);
      
        if(row == mat.size() - 1)
        {
            row = col + 1;
            col = mat.size()- 1;
        }
        else if(col == 0)
        {
            col = row + 1;
            row = 0;
        }
        else
        {
            row++;
            col--;
        }
    }
    return res;
}




int dct_compress(ImageMatrix *img, ofstream &file)
{
    vector< vector < double > > dct_t = transpose(dct_mat);
    init_square_mat(sub_matrix_size, aux);
    vector<vector<double>> prod;
    vector<vector<short>> q;
    int total_size = 0;
    for(int row = 0; row< (int)(img->width); row+=sub_matrix_size)
    {
        for(int col = 0; col < (int)(img->width); col+=sub_matrix_size)
        {
            fill_aux(row, col, img);
            prod = mult_square_mat(dct_mat, aux);
            prod = mult_square_mat(prod, dct_t);
            q = quantize(prod, false);
            vector<short> flat = zig_zag_matrix(q);
            vector<char> writeMe = run_length_encode(flat);
            char size = writeMe.size();
            file.write((const char *)&writeMe[0], size);
            total_size+= size;
        }
    }
    file.close();
    return total_size;
}

template <class T>
vector<T> run_length_decode(vector<T> in)
{
    vector<T> out;
    //print_v(in);
    for(int i = 0; i < in.size(); i++)
    {
        if(in[i] == 0)
        {
            for(int j = 0; j < in[i+1]; j++)
                out.push_back(0);
            i++;
        }
        else
        {
            out.push_back(in[i]);
        }
    }
    //print_v(out);
    cout<<out.size()<<endl;
    return out;
}

void dct_decompress(CompressedImage* img)
{
   img->color_data = run_length_decode(img->color_data);
    
}

//Imprime matriz. format = 'i' para enteros, 'f' para flotantes
template<class T>
void print_mat(vector<vector<T>> mat, char format)
{
    for(int r = 0; r < sub_matrix_size; r++){
        for(int c = 0; c < sub_matrix_size; c++)
        {
            if(format == 'f')
                printf("%7.3f ", mat[r][c]);
            else if(format == 'i')
                 printf("%3i ", mat[r][c]);
        }
        printf("\n");
    }
    printf("\n\n\n");
}

void test()
{
 
    //matriz a comprimir
   vector< vector < double > > test_mat =
    {
        {15, 127, 127, 127, 87, 97, 27, 127},
        {127, 127, 127, 127, 17, 127, 127, 127},
        {127, 20, 127, 127, 77, 127, 127, 127},
        {127, 127, 127, 127, 47, 127, 127, 127},
        {127, 127, 127, 127, 57, 127, 127, 127},
        {127, 127, 127, 127, 17, 127, 127, 127},
        {127, 127, 127, 127, 127, 127, 127, 127},
        {127, 127, 127, 127, 127, 127, 127, 127}
    };

    print_mat(test_mat, 'f');
    //multiplicar dtc * m * dct ^T
    vector< vector < double > > dct_t = transpose(dct_mat);
    vector<vector<double>> prod = mult_square_mat(dct_mat, test_mat);
    
    prod = mult_square_mat(prod, dct_t);
    print_mat(prod, 'f');
    //cuantizar
    vector<vector<short>> q = quantize(prod, false);
    print_mat(q, 'i');
    vector<short> v =  zig_zag_matrix(q);
    
    run_length_decode(run_length_encode(v));
    //Empieza descompresión, multiplicar por coeficientes de cuantización
   /* q = quantize(q, true);
    
    print_mat(q,'i');
    //multiplicar dct ^T * m * dct
    test_mat = mult_square_mat(dct_t, q);
    test_mat = mult_square_mat(test_mat, dct_mat);
    print_mat(test_mat, 'f');*/

}

//segundos actuales
double get_total_time()
{
    struct timeval time;
    gettimeofday(&time,NULL);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
//segundos actuales usados por el CPU ejecutando este proceso
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

int main()
{
    ImageMatrix* img = new ImageMatrix("Images/lena512.bmp");
    ofstream file("compressed.bin", ios::binary);

    //este código hay que moverlo...
    //se escriben los headers al archivo (hay que hacerlo cuando se comprime)
    file.write(reinterpret_cast<const char*>(img->file_header), sizeof(BITMAPFILEHEADER));
    file.write(reinterpret_cast<const char*>(img->info_header), sizeof(BITMAPINFOHEADER));
   //generar la paleta, necesario al momento de la recontsrucción, pero no hay que guardarlo en la comprimirda
/*    for (int i = 0; i < 128; i++) {
        file << (char)i;
        file << (char)i;
        file << (char)i;
        file << (char)0;
    }
    for (int i = -128; i < 0; i++) {
        file << (char)i;
        file << (char)i;
        file << (char)i;
        file << (char)0;
    }
    //imprimir bytes de imagen
    file.write(reinterpret_cast<const char*>(img->matrix), sizeof(char)*img->width*img->height);
*/
    
    
    double wall0 = get_total_time();
    double cpu0  = get_cpu_time();
    int compressed_bytes = dct_compress(img, file);
    double wall1 = get_total_time();
    double cpu1  = get_cpu_time();
    cout << "Se leyeron " << img->size << " bytes en la imagen original."<<endl;
    cout << "Se escribieron " << compressed_bytes <<" bytes que representan los datos de la imagen comprimida." <<endl;
    cout << "Y " << sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) << " de headers del archivo" <<endl;
    //test();
   
    cout << "Tiempo total = " << wall1 - wall0 << endl;
    cout << "Tiempo CPU   = " << cpu1  - cpu0  << endl;

    delete img;
    
    
    CompressedImage* c = new CompressedImage("compressed.bin");
    dct_decompress(c);
    
    delete c;
    return 0;
}

