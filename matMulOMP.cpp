#include <cstdlib>
#include <iostream>
#include "time.h"
#include <omp.h>

using namespace std;

template <typename T> int matrixOutput(T *Mat, int size, string name)
{
    cout << "\"" << name << "\" matrix output:" << endl;
    for (int i=0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            cout << Mat[i*size + j] << " ";
        cout << endl;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Program usage: " << endl;
        cout << "./" << argv[0] << " <n> <threads>" << endl;
        cout << "where <n> is the size of square matrix, <threads> is number of threads" << endl;
        return -1;
    }

    const int N       = atoi(argv[1]);
    const int THREADS = atoi(argv[2]);

    cout << "Matrix size: " << N << "x" << N << endl;
    cout << "Threads: " << THREADS << endl;

    float *A = new float[N*N];
    float *B = new float[N*N];

    for (int i=0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            A[i*N + j] = (i+1) * (j+1);
            B[i*N + j] = (i+1) + 2*(j+1);
        }

    float *C = new float[N*N];

    cout << "Begin calculating ..." << endl;

    omp_set_num_threads(THREADS);
    double wtime = omp_get_wtime();

    int actual_threads = 0;

    #pragma omp parallel shared(actual_threads)
    {
        #pragma omp single
        actual_threads = omp_get_num_threads();

        #pragma omp for
        for (int i=0; i < N; i++)
            for (int j = 0; j < N; j++)
            {
                C[i*N + j] = 0;
                for (int k = 0; k < N; k++)
                    C[i*N + j] += A[i*N + k] * B[k*N + j];
            }
    }

    wtime = omp_get_wtime() - wtime;

    cout << "Num of threads: " << actual_threads << endl;
    cout << "Elapsed omp time: " << wtime << " seconds" << endl;

    delete [] A;
    delete [] B;
    delete [] C;

    return 0;
}