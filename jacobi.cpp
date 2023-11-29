/*
jacobi.cpp*
Created on: 2019-05-03
Author:
*/

#include "jacobi.h"
#include "matrix.h"
#include "sparse_matrix.h"


template <typename TMatrix>
Jacobi<TMatrix>::Jacobi()
{
    // hier nur damp Faktor? Oder geht es sogar ohne, falls man set_damping ausführt?
}


template <typename TMatrix>
bool Jacobi<TMatrix>::init(const vector_type& x)
{
    return true;
}


template <typename TMatrix>
bool Jacobi<TMatrix>::apply(vector_type& c, const vector_type& d) const
{
	// Vektor d enthält den Defekt
	// Vektor c enthält gedämpfte Korrekturen, um den Lösungsvektor zu verbessern

    for(size_t i = 0;i < d.size(); i++){		// über Vektorgröße

        c[i] =  m_damp * d[i] / m_A(i,i);		// d[i] * A^(-1)[i,i] = d[i] / A[i,i]

	}

    return true;
}



// explicit template declarations
template class Jacobi<Matrix>;
template class Jacobi<SparseMatrix>;