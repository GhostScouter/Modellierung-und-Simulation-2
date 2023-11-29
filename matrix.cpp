/*
 * matrix.cpp
 *
 *  Created on: 2019-04-28
 *      Author: 
 */

#include <cassert>
#include <iostream>
#include <iomanip>
#include "matrix.h"

Matrix::Matrix()
{}


Matrix::Matrix(std::size_t r, std::size_t c, double val)
{
	m_rows = r;
    m_cols = c;
    m_mat = Vector(r*c, val);

    //std::cout << m_mat << std::endl;
}

Matrix::Matrix(const Matrix& M){               // mache aus const eine nicht const

    m_mat.resize(M.num_cols() * M.num_rows());
    m_rows = M.num_rows();
    m_cols = M.num_cols();

    for(size_t i = 0; i < M.num_rows(); ++i){
        for(size_t j = 0; j < M.num_cols(); ++j){
            this->operator()(i,j) = M(i,j);
        }
    }

}

Matrix::~Matrix()
{}


std::size_t Matrix::num_rows() const
{
	return m_rows;
}


std::size_t Matrix::num_cols() const
{
	return m_cols;
}



void Matrix::resize(std::size_t r, std::size_t c, double val)
{
    Matrix Inter = Matrix(*this);       // Speichere "original" Matrix zwischen, operiere dann auf "original" Matrix
    //std::cout << Inter << std::endl;
    m_mat = Vector(r*c, val);
    m_rows = r;
    m_cols = c;

    for(size_t i = 0; i < r; ++i){
        for(size_t j = 0; j < c; ++j){
            if(i < Inter.num_rows() && j < Inter.num_cols()) {
                this->operator()(i, j) = Inter(i, j);               // Da wir auf der "original" Matrix arbeiten, muss nichts returned werden
            }
        }
    }
    //std::cout << *this << std::endl;
}



template<bool is_const>
Matrix::RowIteratorBase<is_const>::
RowIteratorBase(typename Matrix::iterator_traits<is_const>::matrix_type& mat, std::size_t rowIndex) : pMat(&mat)
{
	if(mat.num_rows() <= rowIndex) {
        throw std::runtime_error("ugh.. RowIteratorBase was instantiated with rowIndex > mat.num_rows()-1...");
    }
    pMat = &mat;
    pRowIdx = rowIndex;         
    pColIdx = 0;
}


template<bool is_const>
Matrix::RowIteratorBase<is_const>::
RowIteratorBase
(
	typename Matrix::iterator_traits<is_const>::matrix_type& mat,
	std::size_t rowIndex,
	std::size_t startFromCol
)
{
    //std::cout << "initializing iterator with matrix: " << std::endl;
    //std::cout << mat << std::endl;
    if(mat.num_rows() <= rowIndex) {
        throw std::runtime_error("ugh.. RowIteratorBase was instantiated with rowIndex > mat.num_rows()-1...");
    }
    if (mat.num_cols() < startFromCol) {
        throw std::runtime_error("nah... xD RowiteratorBase was instantiated with startFromCol higher than or equal to mat.num_cols().");
    }
    pMat = &mat;
    pRowIdx = rowIndex;
    pColIdx = startFromCol;
}


template<bool is_const>
bool Matrix::RowIteratorBase<is_const>::operator!=(RowIteratorBase& other) const
{
	if (pMat != other.pMat){        // andere Matrix => Ja, sind ungleich
        return true;
    }
    return (pRowIdx != other.pRowIdx || pColIdx != other.pColIdx );     // ansonsten return true falls Rows oder Cols ungleich sind
}


template<bool is_const>
bool Matrix::RowIteratorBase<is_const>::operator==(RowIteratorBase& other) const
{
    if (pMat != other.pMat){
        return false;
    }
    return (pRowIdx == other.pRowIdx && pColIdx == other.pColIdx);
}


template<bool is_const>
Matrix::RowIteratorBase<is_const>& Matrix::RowIteratorBase<is_const>::operator++()
{
	++pColIdx;
    return *this;
}

template<bool is_const>
typename Matrix::iterator_traits<is_const>::entry_type Matrix::RowIteratorBase<is_const>::value() const
{
	return (*pMat)(pRowIdx, pColIdx);

}

template<bool is_const>
typename Matrix::iterator_traits<is_const>::entry_type Matrix::RowIteratorBase<is_const>::value()
{
    return (*pMat)(pRowIdx, pColIdx);
}

template<bool is_const>
std::size_t Matrix::RowIteratorBase<is_const>::col_index() const
{
	return pColIdx;
}



Matrix::RowIterator Matrix::begin(std::size_t r)
{
	return RowIteratorBase<false>(*this, r);
}


Matrix::RowIterator Matrix::end(std::size_t r)
{
    return RowIteratorBase<false>(*this, r, this->num_cols());
}


Matrix::ConstRowIterator Matrix::begin(std::size_t r) const
{
	return RowIteratorBase<true>(*this, r);
}


Matrix::ConstRowIterator Matrix::end(std::size_t r) const
{
	return RowIteratorBase<true>(*this, r, this->num_cols());
}


double Matrix::operator()(std::size_t r, std::size_t c) const   // bei const darf wird eine lokale Variable zurückgegeben
{
	assert(r < m_rows && c < m_cols);
    double out = m_mat[r * m_cols + c];
    return out;

}


double& Matrix::operator()(std::size_t r, std::size_t c)
{
    assert(r < m_rows && c < num_cols());
    return  m_mat[r * m_cols + c];                              // bei nicht const, geben wir die direkte Referenz aus

}

///calculates product M \cdot v = b
Vector Matrix::operator*(Vector v) const
{
	assert(v.size() == m_cols);                             // Vektorgröße muss gleich Spaltenanzahl sein
    Vector b (m_rows, 0);                                   // Ergebnisvektor b

    for(size_t i = 0; i < m_rows; ++i){                     // gehe durch rows
        auto end = this->end(i);                            // Erstelle Iterator an Endposition zum späteren Vergleich
        size_t j = 0;

        for(auto it = this->begin(i); it != end; ++it){     // Falls row-Iterator gleich end Iterator ist: Stopp!
            b[i] += this->operator()(i,j) * v[j];           // Summiere Zwischenergebnis auf, solange wir in derselben Zeile sind
            j++;
        }
    }
    return b;
}

void Matrix::clear(){

    m_mat = Vector(m_rows*m_cols, 0.0);                     // lasse Member Variablen in Ruhe, fülle mit Nullen

}

std::ostream& operator<<(std::ostream& stream, const Matrix& m)
{

    if(m.num_rows() == 0 || m.num_cols()) {return stream << "()";}

    for(size_t r = 0; r < m.num_rows(); ++r){
        for(size_t c = 0; c < m.num_cols(); ++c){
            std::cout << std::setw(6) << std::setfill(' ') << std::setprecision(2) << std::fixed << m(r,c) << "\t";
        }
        std::cout << std::endl;
    }
    return stream;

}


// explicit template instantiations
template class Matrix::RowIteratorBase<true>;
template class Matrix::RowIteratorBase<false>;