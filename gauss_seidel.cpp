/*
 * gauss_seidel.cpp
 *
 *  Created on: 2019-05-03
 *      Author: 
 */

#include "gauss_seidel.h"
#include <iostream>

template <typename TMatrix>
GaussSeidel<TMatrix>::GaussSeidel()
{
	// TODO: implement
}


template <typename TMatrix>
bool GaussSeidel<TMatrix>::init(const vector_type& x)
{

    std::cout << "Rows " << this->m_A->num_rows() << std::endl;
    this->m_A->printMatrix();

	// First, we invert the matrix to be able to work with it?
	
	TMatrix Inverse = TMatrix(this->m_A->num_rows(), this->m_A->num_cols());				// will save M^-1
	TMatrix calculation_slave = TMatrix(this->m_A->num_rows(), this->m_A->num_cols());		// will be used when necessary
    std::cout << "\nInverse 1: " << std::endl << Inverse << std::endl;
	// The Inverse now only contains zeros


	// ########## Calculation of bottom-left entries ###########
	// Implement a Gauss-Jordan algorithm for Matrix Inversion
	double factor = 0;									// will scale each row down, so that it starts with the number 1

    for (int i = 0; i < (this->m_A->num_cols()); ++i) {			    // iterate over Matrix rows

        factor = this->m_A->operator()(i,i);				        // find downscaling factor (ie. matrix diagonal entry)
        calculation_slave(i, i) = 1;			                    // build calculation_slave with diagonal of 1

        std::cout << "Faktor: " << factor << std::endl;

        Inverse(i, i) = 1/factor;                                   // calculate correct diag entry for Inverse

        for (int j = 0; j < this->m_A->num_cols(); ++j) {			// will scale each entry down, so that the rows have a 1 at the diagonal position
			if (i > j){									            // Left-lower part excluding diagonal
				Inverse(i, j) = this->m_A->operator()(i, j) / factor;		// scale entries down
			}
		}
	}

    std::cout << "Inverse mit korrekter Diagonale und skalierten Eintragen in LU: \n"<< std::endl << Inverse << std::endl;
    std::cout << "Calc slave mit Diagonale gleich 1 und rest 0: \n"<< std::endl << calculation_slave << std::endl;

    // Now the Inverse Matrix contains correct values along the diagonal
	// The calculation_slave has the entry 1 along the diagonal
	// All other entries of calculation_slave are scaled down accordingly

	for (int k = 0; k < this->m_A->num_cols(); k++){							// traverse diagonal entries

		for (int i = 0; i < this->m_A->num_cols(); i++){						// in each column

			factor = calculation_slave.operator()(k, k);		                // like before, we find the factor to use for the elimination 	 * this->m_A->num_cols() +

            std::cout << "\nInverse in Schritt: " << k << i << std::endl;
            std::cout << Inverse << std::endl;

			for (int j = 0; j < this->m_A->num_cols(); j++){					// with the factor in hand, we iterate through the row and perfom the elimination
			
				Inverse(i, j) -= factor * calculation_slave(k, k);
				// this line takes an entry Inverse[i][j] and substracts the factor times the corresponding diagonal entry from it
			}
		}
	}
	//Inverse.

    std::cout << "\nInverse: " << std::endl;
    std::cout << Inverse << std::endl;
    std::cout << "I bims eins gaussseidili" << std::endl;
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
				temp_result += Inverse(i, j) * c[j];
			}
		}

		c[i] = temp_result / Inverse(i,i);
	}

    return true;
}


// explicit template declarations
template class GaussSeidel<Matrix>;
template class GaussSeidel<SparseMatrix>;

