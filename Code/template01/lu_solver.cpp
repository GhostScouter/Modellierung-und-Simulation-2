/*
 * lu_solver.cpp
 *
 *  Created on: 2019-05-03
 *      Author: 
 */

#include "lu_solver.h"
#include "matrix.h"
#include "matrix.cpp"
#include "sparse_matrix.h"


template <typename TMatrix>
LUSolver<TMatrix>::LUSolver()
{
    mSolution = Vector();
}


template <typename TMatrix>
LUSolver<TMatrix>::LUSolver(const matrix_type& mat)
{
    //mSolution = Vector(mat.m_cols*mat.m_rows);				// mat ist A

	size_t size = mat.columns;
	Vector LU = mat;  // Initialize LU with the original matrix A

	// Perform LU decomposition
	for (int k = 0; k < size - 1; ++k) {			// Zähler k über Matrixgröße
		for (int i = k + 1; i < size; ++i) {		// für jedes k, ein Zähler i über die restliche Matrixgröße

			if (LU[k][k] == 0.0) {
				std::cerr << "LU decomposition failed: Division by zero." << std::endl;
				return;
			}

			double factor = LU[i][k] / LU[k][k];	// berechne Faktor für L/U
			LU[i][k] = factor;

			for (int j = k + 1; j < size; ++j) {
				LU[i][j] -= factor * LU[k][j];
			}
		}
	}
}


template <typename TMatrix>
void LUSolver<TMatrix>::set_matrix(const matrix_type* A)
{
	// TODO: implement
}


template <typename TMatrix>
bool LUSolver<TMatrix>::init(const vector_type& x)
{
	// TODO: implement
}


template <typename TMatrix>
bool LUSolver<TMatrix>::solve(vector_type& x, const vector_type& b) const
{
	// TODO: implement
}




// explicit template declarations
template class LUSolver<Matrix>;
template class LUSolver<SparseMatrix>;

