#include "vector.h"
#include "matrix.h"
#include "sparse_matrix.h"
#include "vector.cpp"
#include "matrix.cpp"
#include "sparse_matrix.cpp"

#include <iostream>
#include <fstream>
#include <chrono>

int main(int argc, char** argv) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    // Open a file for writing
    std::ofstream outputFile("matrix_multiplication_times.csv");
    
    // Write header to the CSV file
    outputFile << "Matrix_Size,Sparse_Matrix_Multiplication,Regular_Matrix_Multiplication\n";

    for (size_t k = 128; k < 20000; k *= 2) {
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

        duration<double, std::milli> ms_double_sparse = t2 - t1;
        duration<double, std::milli> ms_double_regular = t20 - t10;

        // Append timing information to the CSV file
        outputFile << k << "," << ms_double_sparse.count() << "," << ms_double_regular.count() << "\n";
    }

    // Close the file
    outputFile.close();

    return 0;
}
