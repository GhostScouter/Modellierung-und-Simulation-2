/*
 * Iterative_solver.cpp
 *
 *  Created on: 2019-05-03
 *      Author: 
 */

#include "iterative_solver.h"
#include "matrix.h"
#include "sparse_matrix.h"


template <typename TMatrix>
IterativeSolver<TMatrix>::IterativeSolver(const matrix_type& mat)
{
	// TODO: implement
}


template <typename TMatrix>
IterativeSolver<TMatrix>::IterativeSolver()
{
	// TODO: implement
}


template <typename TMatrix>
void IterativeSolver<TMatrix>::set_corrector(corrector_type* stepMethod)
{
	// TODO: implement
}


template <typename TMatrix>
bool IterativeSolver<TMatrix>::init(const vector_type& x)
{
	// TODO: implement
}


template <typename TMatrix>
void IterativeSolver<TMatrix>::set_matrix(const matrix_type* A)
{
	// TODO: implement
}


template <typename TMatrix>
void IterativeSolver<TMatrix>::set_convergence_params
(
	std::size_t nIter,
	double minDef,
	double minRed
)
{
	// TODO: implement
}


template <typename TMatrix>
bool IterativeSolver<TMatrix>::solve(vector_type& x, const vector_type& b) const
{
	// TODO: implement
}




// explicit template declarations
template class IterativeSolver<Matrix>;
template class IterativeSolver<SparseMatrix>;

