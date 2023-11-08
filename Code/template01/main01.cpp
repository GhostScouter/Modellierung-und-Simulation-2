
#include "vector.h"
#include "matrix.h"
#include "sparse_matrix.h"

#include <iostream>
#include <cstdlib>
#include <chrono>





int main(int argc, char** argv)
{


    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    for(size_t k = 128; k < 50000; k *= 2) {
        Vector v(k, 1.0);
        SparseMatrix S(k, k, 3);
        Matrix M(k, k, 0.0);
        for (size_t i = 0; i < k; ++i) {
            S(i, i) = 2;
            M(i, i) = 2;
            if (i > 0) {
                S(i - 1, i) = -1.0;
                M(i - 1, i) = -1.0;
            }
            if (i < k - 1) {
                S(i + 1, i) = -1;
                M(i + 1, i) = -1;
            }
        }
        auto t1 = high_resolution_clock::now();
        Vector b = S * v;
        auto t2 = high_resolution_clock::now();

        auto t10 = high_resolution_clock::now();
        Vector c = M * v;
        auto t20 = high_resolution_clock::now();

        duration<double, std::milli> ms_double = t2 - t1;
        duration<double, std::milli> ms_double0 = t20 - t10;
        std::cout << "considering square matrix of size " << k << std::endl;
        std::cout << "duration of S * v: " << ms_double.count() << " ms" << std::endl;
        std::cout << "duration of M * v: " << ms_double0.count() << " ms" << "\n" <<  std::endl;
    }
    return 0;
}

