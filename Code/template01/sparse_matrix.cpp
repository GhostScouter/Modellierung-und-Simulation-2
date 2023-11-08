/*
 * sparse_matrix.cpp
 *
 *  Created on: 2019-04-28
 *      Author: 
 */

#include "sparse_matrix.h"
#include <iostream>

using namespace std;

SparseMatrix::SparseMatrix()
{
	capacity = 5;
	rows = 0;
	columns = 0;

	vectorSize = capacity * rows;
	colInds = new size_t [vectorSize];
	values = new double [vectorSize];

}


SparseMatrix::SparseMatrix(std::size_t r, std::size_t c, std::size_t rowCapacity)
{
	vectorSize = rowCapacity * r;
	capacity = rowCapacity;
	rows = r;
	columns = c;

	colInds = new size_t [vectorSize];
	values = new double [vectorSize];
	for(int i=0; i < vectorSize; ++i){
		colInds[i] = (size_t)-1;
		values[i] = 0;
	}
}


SparseMatrix::~SparseMatrix()
{
    delete[] colInds;
    delete[] values;
}


std::size_t SparseMatrix::num_rows() const
{
	return rows;
}


std::size_t SparseMatrix::num_cols() const
{
	return columns;
}


SparseMatrix& SparseMatrix::printMatrix(){

	cout << "Hier ist der ColInds-Vektor: " << endl;

	for(size_t i=0; i < vectorSize; i++){
		cout << colInds[i] << ", ";
	}
	cout << endl << "Hier ist der Values-Vektor: " << endl;
	for(size_t i=0; i < vectorSize; i++){
		cout << values[i] << ", ";
	}
	cout << endl << endl;
} 


void SparseMatrix::resize(std::size_t r, std::size_t c, double defVal)
{

	size_t newRows = r;
	size_t newColumns = c;

	size_t newVectorSize = newRows * capacity;

	double* newColInds = new double[newVectorSize];
	double* newValues = new double[newVectorSize];

	// ################################################################################
	// 					Erster Fall: neue Reihenanzahl ist kleiner
	// ################################################################################

	if (newRows < rows){							// neue Reihenanzahl ist kleiner
		for(size_t i=0; i < newVectorSize; i++){	// fülle neue Arrays auf, bis zur neuen Grenze (newRows*capacity)
			newColInds[i] = colInds[i];
			newValues[i] = values[i];
		}
	
	// für den Fall, dass neben den Rows, auch noch Columns gelöscht werden sollen,
	// können wir erst die Rows an Ende "abhacken" und danach durch die noch existierenden Rows gehen,
	// und die column-Einträge löschen, die in Frage kommen.

		if (newColumns < columns){						// falls Columns gelöscht werden sollen

			for(size_t i=0; i < newVectorSize; i++){	// gehe die beiden Vektoren bis zum neuen Ende durch

				if (colInds[i] < newColumns){			// prüfe im alten oder neuen Vektor, ob Informationen eines gelöschten Columns existieren
					newColInds[i] = -1;					// lösche Information, ob ein Wert existiert
					newValues[i] = 0;					// lösche Information, welcher Wert existiert
				}
			}
		}
		else{
			// falls keine Columns gelöscht werden sollen, wird nichts unternommen.
		}
	}

	// ################################################################################
	// Zweiter Fall: Ab hier ist nun die neue Reihenanzahl größer oder gleich der alten
	// ################################################################################

	else{

	// Wir prüfen zuerst, ob Columns gelöscht werden sollen, damit wir nicht den größeren Array durchgehen müssen

		if (newColumns < columns){						// falls Columns gelöscht werden sollen

			for(size_t i=0; i < newVectorSize; i++){	// gehe die beiden Vektoren bis zum neuen Ende durch

				if (colInds[i] < newColumns){			// prüfe im alten oder neuen Vektor, ob Informationen eines gelöschten Columns existieren
					newColInds[i] = -1;					// lösche Information, ob ein Wert existiert
					newValues[i] = 0;					// lösche Information, welcher Wert existiert
				}
			}
		}
		else{
			// falls keine Columns gelöscht werden sollen, wird nichts unternommen.
		}
		// egal, ob Coloumns gelöscht wurden, oder nicht, sind ja immernoch Rows hinzugekommen!
		// Füge diese Einträge nun hinzu

		for(int i=0; i < newVectorSize; i++){	// fülle neue Arrays mit Nullen
			newColInds[i] = -1;
			newValues[i] = 0;
		}

		for(int i=0; i < vectorSize; i++){	// fülle hier neue Arrays mit den alten Werten. Dazugekommener Platz ist nun mit Nullen besetzt.
			newColInds[i] = colInds[i];
			newValues[i] = values[i];
		}
	}
	

    // Deallocate the old arrays
    delete[] colInds;
    delete[] values;

    // Set the arrays to the new arrays
	colInds = newColInds;
	values = newValues;
    rows = r;
	columns = c;
	vectorSize = newVectorSize;
}



template<bool is_const>
SparseMatrix::RowIteratorBase<is_const>::RowIteratorBase
(
	typename iterator_traits<is_const>::matrix_type& mat,
	std::size_t rowIndex
)
{
	// todo: implement
}


template<bool is_const>
SparseMatrix::RowIteratorBase<is_const>::RowIteratorBase
(
	typename SparseMatrix::iterator_traits<is_const>::matrix_type& mat,
	std::size_t rowIndex,
	std::size_t startFromCol
)
{
	// todo: implement
}


template<bool is_const>
bool SparseMatrix::RowIteratorBase<is_const>::operator!=(RowIteratorBase& other) const
{
	// todo: implement
}


template<bool is_const>
bool SparseMatrix::RowIteratorBase<is_const>::operator==(RowIteratorBase& other) const
{
	// todo: implement
}


template<bool is_const>
SparseMatrix::RowIteratorBase<is_const>& SparseMatrix::RowIteratorBase<is_const>::operator++()
{
	// todo: implement
}

template<bool is_const>
typename SparseMatrix::iterator_traits<is_const>::entry_type SparseMatrix::RowIteratorBase<is_const>::value() const
{
	// todo: implement
}

template<bool is_const>
typename SparseMatrix::iterator_traits<is_const>::entry_type& SparseMatrix::RowIteratorBase<is_const>::value()
{
	// todo: implement
}

template<bool is_const>
std::size_t SparseMatrix::RowIteratorBase<is_const>::col_index() const
{
	// todo: implement
}



SparseMatrix::RowIterator SparseMatrix::begin(std::size_t r)
{
	// todo: implement
}


SparseMatrix::RowIterator SparseMatrix::end(std::size_t r)
{
	// todo: implement
}


SparseMatrix::ConstRowIterator SparseMatrix::begin(std::size_t r) const
{
	// todo: implement
}


SparseMatrix::ConstRowIterator SparseMatrix::end(std::size_t r) const
{
	// todo: implement
}


bool SparseMatrix::has_entry(std::size_t r, std::size_t c) const
{
	// todo: implement
}


double SparseMatrix::operator()(std::size_t r, std::size_t c) const
{
	// todo: implement
}


double& SparseMatrix::operator()(std::size_t r, std::size_t c)
{
	// todo: implement
}


Vector SparseMatrix::operator*(Vector v) const
{
	// todo: implement
}



std::ostream& operator<<(std::ostream& stream, const SparseMatrix& m)
{
	// todo: implement
}




// explicit template instantiations
template class SparseMatrix::RowIteratorBase<true>;
template class SparseMatrix::RowIteratorBase<false>;

int main()
{

	SparseMatrix brot (3,3,2);
	brot.printMatrix();
	
	brot.resize(4,4,0);
	brot.printMatrix();

}