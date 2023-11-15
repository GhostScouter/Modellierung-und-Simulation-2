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

	size_t size = mat.m_cols;
	Vector LU = mat;  									// Initialize LU with the original matrix A

	// Perform LU decomposition
	for (int k = 0; k < mat.m_cols - 1; ++k) {			// Zähler k über Matrixgröße
		for (int i = k + 1; i < mat.m_rows; ++i) {		// für jedes k, ein Zähler i über die restliche Matrixgröße

			size_t comparisionEntry = k * mat.m_cols + k * mat.m_rows;			// next entry to compare to and calculate factor

			if (LU[comparisionEntry] == 0.0) {
				std::cerr << "LU decomposition failed: Division by zero." << std::endl;
				return;
			}

			double factor = LU[(i*mat.m_cols)+k] / LU[comparisionEntry];		// berechne Faktor für L/U
			LU[(i*mat.m_cols)+k] = factor;										// schreibe Faktor an die Stelle

			}
		for (int j = 0; j < k + 1; j++) {		// wende die Änderung auf die Matrix an und berechne R-Matrix Werte
			LU[comparisionEntry + j] -= factor * LU[comparisionEntry - mat.m_cols];

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

int main(){
	return 0;
}