/*
 * jacobi.cpp
 *
 *  Created on: 2019-05-03
 *      Author: 
 */

#include "jacobi.h"
#include "matrix.h"
#include "sparse_matrix.h"


template <typename TMatrix>
Jacobi<TMatrix>::Jacobi()
{
	// TODO: implement
}


template <typename TMatrix>
bool Jacobi<TMatrix>::init(const vector_type& x)
{
	return true;
}


template <typename TMatrix>
bool Jacobi<TMatrix>::apply(vector_type& c, const vector_type& d) const
{
	
	for(size_t i = 0;i < d.size(); i++){
		c[i] =  m_damp * d[i] / m_A(i,i);
	}
	return true;
}



// explicit template declarations
template class Jacobi<Matrix>;
template class Jacobi<SparseMatrix>;
