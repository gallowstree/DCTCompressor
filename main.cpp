#define _USE_MATH_DEFINES
#include "math.h"
#include "stdio.h"
#include "ImageMatrix.h"
#include <sys/time.h>
//#define DEBUG

using namespace std;

vector< vector<double> > aux;

int sub_matrix_size;

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

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


void init_square_mat(int size, vector<vector<double>> &mat)
{
    mat.reserve(size);
    for ( int i = 0 ; i < size ; i++ )
        mat[i].resize(size);
}

vector<vector<double>> mult_square_mat(vector<vector<double>> &mat1, vector<vector<double>> &mat2)
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
                product[row][r_col] += mat1[row][col]*mat2[col][r_col];
            }
        }
    }
    return product;
}

vector<vector<double>> transpose(vector<vector<double>> &mat)
{
    int r_size = mat.size();
    int c_size = mat[0].size();
    vector<vector<double>> outtrans(c_size, vector<double>(r_size));
    for (int i = 0; i < r_size; ++i)
        for (int j = 0; j < c_size; ++j)
            outtrans[j][i] = mat[i][j];
    return outtrans;
}

int main()
{
    sub_matrix_size = 8;
    init_square_mat(sub_matrix_size, aux);
    ImageMatrix* img = new ImageMatrix("/Users/alejandroalvarado/cc4/DCTCompressor/Images/lena512.bmp");
    
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
    vector< vector < double > > dct_t = transpose(dct_mat);
    vector< vector < double > > test_mat =
    {
        {26, -5,  -5,  -5, -5, -5, -5,  8},
        {64,  52,  8,  26, 26, 26, 8, -18},
        {126,  70, 26, 26, 52, 26,  -5 , -5},
        {111, 52, 8, 52,  52,  38,  -5, -5},
        {52, 26, 8,  39,  38, 21, 8,  8},
        {0, 8,  -5,  8, 26, 52,  70, 26},
        {-5, -23,  -18, 21, 8,  8, 52,  38},
        {-18, 8,  -5, -5,  -5, 8,  26 ,8}
    };
    
    double wall0 = get_wall_time();
    double cpu0  = get_cpu_time();
    vector<vector<double>> prod;
    for(int row = 0; row< (int)(img->width); row+=sub_matrix_size)
    {
        for(int col = 0; col < (int)(img->width); col+=sub_matrix_size)
        {
            fill_aux(row, col, img);
            prod = mult_square_mat(dct_mat, aux);
            //prod = mult_square_mat(dct_mat, test_mat);
            prod = mult_square_mat(prod, dct_t);
        }

    }
    double wall1 = get_wall_time();
    double cpu1  = get_cpu_time();
    
    cout << "Wall Time = " << wall1 - wall0 << endl;
    cout << "CPU Time  = " << cpu1  - cpu0  << endl;

    
    return 0;
}

