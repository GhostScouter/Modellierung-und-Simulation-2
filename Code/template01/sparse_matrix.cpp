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

	colInds = new double [vectorSize];
	values = new double [vectorSize];
	for(int i=0; i < vectorSize; ++i){
		colInds[i] = -1;
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

	size_t* newColInds = new size_t[newVectorSize];
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

		for(int i=0; i < newVectorSize; i++){	// fülle neue Arrays mit Nullen
			newColInds[i] = -1;
			newValues[i] = 0;
		}

		for(int i=0; i < vectorSize; i++){	// fülle hier neue Arrays mit den alten Werten. Dazugekommener Platz ist nun mit Nullen besetzt.
			newColInds[i] = colInds[i];
			newValues[i] = values[i];
		}


		if (newColumns < columns){						// falls Columns gelöscht werden sollen

			for(size_t i=0; i < newVectorSize; i++){	// gehe die beiden Vektoren bis zum neuen Ende durch

				if (colInds[i] > newColumns){			// prüfe im alten oder neuen Vektor, ob Informationen eines gelöschten Columns existieren
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
	vectorPosition = (rowIndex - 1) * capacity;
	fakeColumnPosition = 0;
}


template<bool is_const>
SparseMatrix::RowIteratorBase<is_const>::RowIteratorBase
(
	typename SparseMatrix::iterator_traits<is_const>::matrix_type& mat,
	std::size_t rowIndex,
	std::size_t startFromCol
)
{
	for (size_t i; ((rowIndex-1)*capacity) < i <(rowIndex*capacity); i++){		// suche im Bereich der Reihe nach Eintrag
		if (colInds[i] = startFromCol){
			vectorPosition = i;
		}
		else if (colInds[i] > startFromCol){									// ein größerer Eintrag gefunden => Die Spalte ist leer!
			vectorPosition = i;													// gehe in den nächsten Eintrag um Fortschritt zu garantieren
		}
		else{
			vectorPosition = (rowIndex - 1) * capacity;							// bleibe in Zeile am ersten Eintrag. Der ++-operator stellt eine effektive Suche sicher
		}
	}
	fakeColumnPosition = startFromCol;
}


template<bool is_const>
bool SparseMatrix::RowIteratorBase<is_const>::operator!=(RowIteratorBase& other) const
{
	bool result = false;

	if (vectorPosition != other.vectorPosition){
		result = true;
	}
	return result;
}


template<bool is_const>
bool SparseMatrix::RowIteratorBase<is_const>::operator==(RowIteratorBase& other) const
{
	bool result = false;

	if (vectorPosition == other.vectorPosition){
		result = true;
	}
	return result;
}

template<bool is_const>
SparseMatrix::RowIteratorBase<is_const>& SparseMatrix::RowIteratorBase<is_const>::operator++()
{
	fakeColumnPosition = fakeColumnPosition + 1;			// wir suchen nun eine Spalte weiter rechts

	if (fakeColumnPosition > columns - 1){					// wenn wir weiter rechts sind, als wir Spalten haben, müssen wir "in die nächste Zeile"
		fakeColumnPosition = 0;								// nur jetzt, dürfen wir in den nächsten "Zeilenabschnitt" im Vektor
	}

	if (fakeColumnPosition <= colInds[vectorPosition]){		// wenn wir an richtiger Stelle sind, dannverweilen wir
		vectorPosition = vectorPosition;
	}

	else{
		// wir wollen nun eins weiter! Das dürfen wir grundsätzlich.
		// Ausnahme: Wir sind am letzten ColInds Eintrag einer Zeile!
		// 		Daher: Prüfe zuerst, ob wir an einer solchen "Grenze" sind
		// 		Falls ja, dürfen wir nicht unsere vectorposition erhöhen
		// 			Ausnahme der Ausnahme: Falls wir einen Passiergschein haben:
		//			aka. (fakeColumnPosition = 0) aka. Reihenende der Matrix erreicht, dann dürfen wir weiter

		if ((vectorPosition + 1) % capacity == 0){
			
			if (fakeColumnPosition = 0){
				vectorPosition = vectorPosition + 1;
			}

			else{
				vectorPosition = vectorPosition;
			}

		}
		else{
			// wir wollen weiter UND sind nicht an der Grenze:
			vectorPosition = vectorPosition + 1;
		}
	}
}

template<bool is_const>
typename SparseMatrix::iterator_traits<is_const>::entry_type SparseMatrix::RowIteratorBase<is_const>::value() const
{
	if(fakeColumnPosition == colInds[vectorPosition]){

		return values[vectorPosition];
	}
	else{
		return 0;
	}

}

template<bool is_const>
typename SparseMatrix::iterator_traits<is_const>::entry_type& SparseMatrix::RowIteratorBase<is_const>::value()
{
	if(fakeColumnPosition == colInds[vectorPosition]){

		return values[vectorPosition];
	}
	else{
		return 0;
	}
}

template<bool is_const>
std::size_t SparseMatrix::RowIteratorBase<is_const>::col_index() const
{
	return fakeColumnPosition;
}



SparseMatrix::RowIterator SparseMatrix::begin(std::size_t r)
{
	return SparseMatrix::RowIterator(*this, r);
}


SparseMatrix::RowIterator SparseMatrix::end(std::size_t r)
{
	return SparseMatrix::RowIterator(*this, r, columns);
}


SparseMatrix::ConstRowIterator SparseMatrix::begin(std::size_t r) const
{
	return SparseMatrix::ConstRowIterator(*this, r);
}


SparseMatrix::ConstRowIterator SparseMatrix::end(std::size_t r) const
{
	return SparseMatrix::ConstRowIterator(*this, r, columns);
}


bool SparseMatrix::has_entry(std::size_t r, std::size_t c) const
{
	for (size_t i; ((r-1)*capacity) < i <(r*capacity); i++){
		if(colInds[i] == c){
			return true;
		}
		else{
			return false;
		}
	}
}


double SparseMatrix::operator()(std::size_t r, std::size_t c) const
{
	for (size_t i; ((r-1)*capacity) < i <(r*capacity); i++){
		if(colInds[i] == c){
			return values[i];
		}
	}
	return 0;
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

	SparseMatrix brot (5,5,2);
	brot.printMatrix();
	
	brot.resize(10,2,0);
	brot.printMatrix();

}
