/*
 * matrix.cpp
 *
 *  Created on: 2019-04-28
 *      Author: 
 */

#include "matrix.h"
#include "vector.h"
#include "vector.cpp"

Matrix::Matrix()
{
	rows = 0;
	columns = 0;

	matrix = new double* [rows];
	for(int i=0; i < rows; ++i)
		matrix[i] = new double [columns];
}


Matrix::Matrix(std::size_t r, std::size_t c, double val)
{
	rows = r;
	columns = c;

	matrix = new double* [rows];
	for(int i=0; i < rows; ++i)
		matrix[i] = new double [columns];

	for(int j=0; j < rows; ++j)
		for(int k=0; k < columns; ++k)
			matrix[j][k] = val;
}


Matrix::~Matrix(){

    for (std::size_t i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

}
Matrix& Matrix::printMatrix(){
	std::cout << matrix << std::endl;
	int counter = 0;
	for(int i=0; i < rows; ++i){
		counter = 0;
		std::cout << "|";
		for(int j=0; j < columns; ++j){
			if (counter < (columns-1)){
				std::cout << matrix[i][j];
				counter +=1;
			}
			else{
				std::cout << matrix[i][j] << "|" << std::endl;
				counter +=1;
			}
		}
	}

}


std::size_t Matrix::num_rows() const
{
	return rows;
}


std::size_t Matrix::num_cols() const
{
	
	return columns;
}


void Matrix::resize(std::size_t r, std::size_t c, double val)
{
	// Allocate a new matrix with the new size
    double** newMatrix = new double*[r];
    for (std::size_t i = 0; i < r; ++i) {
        newMatrix[i] = new double[c];
        // Initialize with the value provided or the default value
        for (std::size_t j = 0; j < c; ++j) {
            newMatrix[i][j] = val;
        }
    }

    // Copy the data from the old matrix to the new matrix
    // Only copy the data if it is within the bounds of both the old and new matrix
    for (std::size_t i = 0; i < r && i < rows; ++i) {
        for (std::size_t j = 0; j < c && j < columns; ++j) {
            newMatrix[i][j] = matrix[i][j];
        }
    }

    // Deallocate the old matrix
    for (std::size_t i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    // Set the matrix to the new matrix
    matrix = newMatrix;
    rows = r;
	columns = c;
}


template<bool is_const>
Matrix::RowIteratorBase<is_const>::RowIteratorBase(typename Matrix::iterator_traits<is_const>::matrix_type& mat, std::size_t rowIndex)
{
	content = 0;
	rowPosition = rowIndex;
	colPosition = 0;
}


template<bool is_const>
Matrix::RowIteratorBase<is_const>::RowIteratorBase(typename Matrix::iterator_traits<is_const>::matrix_type& mat,std::size_t rowIndex,std::size_t startFromCol)
: underlyingMatrix(&mat)
{
	content = 0;
	rowPosition = rowIndex;
	colPosition = startFromCol;
}


template<bool is_const>
bool Matrix::RowIteratorBase<is_const>::operator!=(RowIteratorBase& other) const
{
	bool result = false;

	if (rowPosition != other.rowPosition or colPosition != other.colPosition){
		result = true;
	}
	return result;
}


template<bool is_const>
bool Matrix::RowIteratorBase<is_const>::operator==(RowIteratorBase& other) const
{
	// todo: implement
	bool result = false;

	if (rowPosition == other.rowPosition and colPosition == other.colPosition){
		result = true;
	}
	return result;
}


template<bool is_const>
Matrix::RowIteratorBase<is_const>& Matrix::RowIteratorBase<is_const>::operator++()
{


	colPosition += 1;
	content = underlyingMatrix -> matrix[rowPosition][colPosition];
}

template<bool is_const>
typename Matrix::iterator_traits<is_const>::entry_type Matrix::RowIteratorBase<is_const>::value() const
{
	std::cout << "Hallo, bin konstant. Hier ist der Wert an der Stelle des Iterators: " << content;
}

template<bool is_const>
typename Matrix::iterator_traits<is_const>::entry_type& Matrix::RowIteratorBase<is_const>::value()
{
	return underlyingMatrix -> matrix[rowPosition][colPosition];
}

template<bool is_const>
std::size_t Matrix::RowIteratorBase<is_const>::col_index() const
{
	return colPosition;
}


Matrix::RowIterator Matrix::begin(std::size_t r)
{
    // Initialize a RowIterator object with the current Matrix and the specified row
    return Matrix::RowIterator(*this, r);
}



Matrix::RowIterator Matrix::end(std::size_t r)
{
    // Initialize a RowIterator object with the current Matrix and the specified row
    return Matrix::RowIterator(*this, r, columns);
}


Matrix::ConstRowIterator Matrix::begin(std::size_t r) const
{
    // Initialize a RowIterator object with the current Matrix and the specified row
    return Matrix::ConstRowIterator(*this, r);
}


Matrix::ConstRowIterator Matrix::end(std::size_t r) const
{
    // Initialize a RowIterator object with the current Matrix and the specified row
    return Matrix::ConstRowIterator(*this, r, columns);
}


double Matrix::operator()(std::size_t r, std::size_t c) const
{
	return matrix[r][c];
}


double& Matrix::operator()(std::size_t r, std::size_t c)
{
	return matrix[r][c];
}


Vector Matrix::operator*(Vector v) const
{
	double temporaryResult = 0;				// used in the core of the calculation to sum up entries of result vector
	int size = v.vectorSize;

	Vector gatherResults = Vector(size);	// build result vector to be altered and then returned

    for (int i = 0; i < rows; ++i){			// pass through the matrix' rows
		for (int j = 0; j < size; j++){		// calculate one entry of the result vector

			temporaryResult = temporaryResult + matrix[i][j] * v.vector[j];
		}
		gatherResults.vector[i] = temporaryResult;		// put result into result vector
		temporaryResult = 0;							// reset the temporaryResult so the next calculation can take place
	}

	return gatherResults;
}

// explicit template instantiations
template class Matrix::RowIteratorBase<true>;
template class Matrix::RowIteratorBase<false>;

int main() {

	Matrix testMatrix(1,1,1);
	testMatrix.resize(2,2,2);
	testMatrix.resize(3,3,3);
	Vector testVektor(3,3);
	
	Matrix::RowIteratorBase<false> test_01(testMatrix, 1, 1);
	double x = test_01.value();
	std::size_t colin =  test_01.col_index();
	std::cout << "colin: " << colin << std::endl;
	std::cout << "x: " << x << std::endl;

	testMatrix.printMatrix();
	testVektor.printVector();

	Vector result = testMatrix.operator*(testVektor);

	result.printVector();

	return 0;
}