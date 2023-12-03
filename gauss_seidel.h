/*
 * gauss_seidel.cpp
 *
 *  Created on: 2019-05-03
 *      Author:
 */

#include "gauss_seidel.h"
#include "matrix.h"
#include "sparse_matrix.h"


template <typename TMatrix>
GaussSeidel<TMatrix>::GaussSeidel()
{
    // TODO: implement
}


template <typename TMatrix>
bool GaussSeidel<TMatrix>::init(const vector_type& x)
{
    // hier invertieren?
    Matrix A = *(this->m_A);												// copies the const Matrix A
    Matrix Inverse = Matrix(A.m_rows, A.m_cols, 0.0);				// will save M^-1
    Matrix calculation_slave = Matrix(A.m_rows, A.m_cols, 0.0);		// will be used when necessary


    // ############## Set R-part to 0 ##############
    // The Inverse already only contains zeros


    // ########## Calculation of bottom-left entries ###########
    // Implement a Gauss-Jordan algorithm for Matrix Inversion

    double factor = 0;									// will scale each row down, so that it starts with the number 1

    for (int i = 0; i < (m_A->m_cols); ++i) {			// iterate over Matrix rows
        factor = A[i*A.m_cols];							// find downscaling factor (ie. matrix diagonal entry)

        calculation_slave[i*A.m_cols] = 1;				// build calculation_slave with diagonal of 1
        calculation_slave[i*A.m_cols] = 1;				// build calculation_slave with diagonal of 1

        for (int j = 0; j < A.m_cols; ++j) {			// will scale each entry down, so that the rows have a 1 at the diagonal position
            if (i > j){
                Inverse[i*A.m_cols + j] = A[i*A.m_cols + j] / factor;		// scale entries down
            }
        }
    }

    // Now the Inverse Matrix contains correct values along the diagonal
    // The calculation_slave has the entry 1 along the diagonal
    // All other entries of calculation_slave are scaled down accordingly

    for (int k = 0; k < A.m_cols; k++){							// traverse diagonal entries

        for (int i = 0; i < A.m_cols; i++){						// in each column

            factor = calculation_slave[i * A.m_cols + k];		// like before, we find the factor to use for the elimination

            for (int j = 0; j < A.m_cols; j++){					// with the factor in hand, we iterate through the row and perfom the elimination

                Inverse[i * A.m_cols + j] -= factor * calculation_slave[i * A.m_cols + k];
                // this line takes an entry Inverse[i][j] and substracts the factor * entry from it
            }
        }
    }

    return true;
}


template <typename TMatrix>
bool GaussSeidel<TMatrix>::apply(vector_type& c, const vector_type& d) const
{
    // Vektor d enthält den Defekt
    // Vektor c enthält Korrekturen, um den Lösungsvektor zu verbessern

    for (size_t i = 0; i < c.size(); ++i) {		// iterate through
        double temp_result = 0.0;				// set temp_result to 0

        for (size_t j = 0; j < c.size(); ++j) {	// gehe entsprechend der Aufgabe die Zeilen teilweise durch

            if (j <= i) {						// Linke untere Dreiecksmatrix inklusive Diagonale
                temp_result += Inverse[i * Inverse.m_cols + j] * c[j];
            }
        }

        c[i] = temp_result / Inverse[i][i];
    }

    return true;
}


// explicit template declarations
template class GaussSeidel<Matrix>;
template class GaussSeidel<SparseMatrix>;