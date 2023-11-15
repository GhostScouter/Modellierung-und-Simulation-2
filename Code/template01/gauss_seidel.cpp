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
	// TODO: implement
}


template <typename TMatrix>
bool GaussSeidel<TMatrix>::apply(vector_type& c, const vector_type& d) const
{
	// TODO: implement
}


// explicit template declarations
template class GaussSeidel<Matrix>;
template class GaussSeidel<SparseMatrix>;

