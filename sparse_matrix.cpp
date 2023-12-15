/*
 * sparse_matrix.cpp
 *
 *  Created on: 2019-04-28
 *      Author: 
 */

#include "sparse_matrix.h"
#include <iostream>
#include <iomanip>


SparseMatrix::SparseMatrix()
{
    m_rows = 0;
    m_cols = 0;
    m_row_capacity = 9;
    m_values = std::vector<double>(0, m_zero);
    m_col_inds = std::vector<size_t>(m_row_capacity * m_rows, (size_t) - 1);
}


SparseMatrix::SparseMatrix(std::size_t r, std::size_t c, std::size_t rowCapacity)
{
    m_rows = r;
    m_cols = c;
    m_row_capacity = rowCapacity;
    m_values = std::vector<double>(m_row_capacity * m_rows, m_zero);
    m_col_inds = std::vector<size_t>(m_row_capacity * m_rows, (size_t) - 1);
}


SparseMatrix::~SparseMatrix()
{}

void SparseMatrix::printMatrix() const{

    int c = 1;
    std::cout << "Hier die Sparsematrix: " << std::endl;

    std::cout << "Spaltenanzahl: " << m_cols << std::endl;
    std::cout << "Reihenanzahl:  " << m_rows << std::endl;
    std::cout << "Row capacity:  " << m_row_capacity << std::endl;
    for(size_t i=0; i < m_row_capacity*m_rows; i++){
        std::cout << m_col_inds[i] << ", ";
        if (c % m_row_capacity == 0){
            std::cout << std::endl;
        }
        c += 1;
    }
    std::cout << std::endl;
    c = 1;
    for(size_t i=0; i < m_row_capacity*m_rows; i++){
        std::cout <<m_values[i] << ", ";
        if (c % m_row_capacity == 0){
            std::cout << std::endl;
        }
        c += 1;
    }
}

std::size_t SparseMatrix::num_rows() const
{
    return m_rows;
}


std::size_t SparseMatrix::num_cols() const
{
    return m_cols;
}


void SparseMatrix::resize(std::size_t r, std::size_t c, double defVal)
{
    m_values.resize(m_row_capacity * r, defVal);
    m_col_inds.resize(m_row_capacity * r, (size_t) - 1);

    m_rows = r;
    if(c >= m_cols){
        m_cols = c;
        return;
    }

    m_cols = c;
    for(size_t i = 0; i < m_row_capacity * r; ++i){
        if(m_col_inds[i] < m_cols){
            continue;
        }
        m_col_inds[i] = (size_t) -1;
    }
}



template<bool is_const>
SparseMatrix::RowIteratorBase<is_const>::RowIteratorBase
        (
                typename iterator_traits<is_const>::matrix_type& mat,
                std::size_t rowIndex
        )
{
    pMat = &mat;
    if(pMat->num_rows() <= rowIndex){
        throw std::runtime_error("\"ugh.. RowIteratorBase was instantiated with rowIndex > mat.num_rows()-1...");
    }
    pCurInd = &mat.m_col_inds[rowIndex * mat.m_row_capacity];
    pCurVal = &mat.m_values[rowIndex * mat.m_row_capacity];
}


template<bool is_const>
SparseMatrix::RowIteratorBase<is_const>::RowIteratorBase
        (
                typename SparseMatrix::iterator_traits<is_const>::matrix_type& mat,
                std::size_t rowIndex,
                std::size_t startFromCol
        )
{
    pCurInd = &mat.m_col_inds[rowIndex * mat.m_row_capacity];
    pCurVal = &mat.m_values[rowIndex * mat.m_row_capacity];
    for(size_t i = 0; i < mat.m_row_capacity; ++i){
        if(*pCurInd == startFromCol){
            break;
        }
        ++pCurInd;
        ++pCurVal;
    }
}


template<bool is_const>
bool SparseMatrix::RowIteratorBase<is_const>::operator!=(RowIteratorBase& other) const
{
    return pCurInd != other.pCurInd;
}


template<bool is_const>
bool SparseMatrix::RowIteratorBase<is_const>::operator==(RowIteratorBase& other) const
{
    return pCurInd == other.pCurInd;
}


template<bool is_const>
SparseMatrix::RowIteratorBase<is_const>& SparseMatrix::RowIteratorBase<is_const>::operator++()
{
    ++pCurInd;
    ++pCurVal;
    return *this;
}

template<bool is_const>
typename SparseMatrix::iterator_traits<is_const>::entry_type SparseMatrix::RowIteratorBase<is_const>::value() const
{
    return *pCurVal;
}

template<bool is_const>
typename SparseMatrix::iterator_traits<is_const>::entry_type& SparseMatrix::RowIteratorBase<is_const>::value()
{
    return *pCurVal;
}

template<bool is_const>
std::size_t SparseMatrix::RowIteratorBase<is_const>::col_index() const
{
    return *pCurInd;
}



SparseMatrix::RowIterator SparseMatrix::begin(std::size_t r)
{
    return RowIterator{*this, r};
}


SparseMatrix::RowIterator SparseMatrix::end(std::size_t r)
{
    return RowIterator{*this, r, (size_t) -1};
}


SparseMatrix::ConstRowIterator SparseMatrix::begin(std::size_t r) const
{
    return ConstRowIterator{*this, r};
}


SparseMatrix::ConstRowIterator SparseMatrix::end(std::size_t r) const
{
    return ConstRowIterator{*this, r, (size_t) -1};
}


bool SparseMatrix::has_entry(std::size_t r, std::size_t c) const
{
    if(r >= m_rows || c >= m_cols){
        throw std::runtime_error("matrix request out of bounds...");
    }
    for(size_t i = 0; i < m_row_capacity; ++i){
        if(m_col_inds[r * m_row_capacity + i] == c){
            return true;
        }
    }
    return false;
}


double SparseMatrix::operator()(std::size_t r, std::size_t c) const
{
    for(size_t i = 0; i < m_row_capacity; ++i){
        if(m_col_inds[r * m_row_capacity + i] == c){
            return m_values[r * m_row_capacity +i];
        }
    }
    return m_zero;
}


double& SparseMatrix::operator()(std::size_t r, std::size_t c)
{
    //find entry if it exists
    size_t nr_row_elements = 0;
    for(size_t i = 0; i < m_row_capacity; ++i){
        if(m_col_inds[r * m_row_capacity + i] < (size_t) -1){
            ++nr_row_elements;
        }
        if(m_col_inds[r * m_row_capacity + i] == c){
            return m_values[r * m_row_capacity +i];
        }
    }
    //did not find entry. check whether it can be created:
    /*
    if(nr_row_elements == m_row_capacity){
        throw std::runtime_error("matrix entry not found. creation of new entry frobidden by row capacity...");
    }
     */
    //since col indices are sorted, last entry has to be free
    size_t end_of_r = (r + 1) * m_row_capacity - 1;
    m_col_inds[end_of_r] = c;
    //indices will be out of order -> small bubblesort to fix it (will need only one iteration! ;) )
    size_t offset = 0;
    for(size_t i = 0; i < m_row_capacity - 1; ++i){
        if(m_col_inds[end_of_r - i] < m_col_inds[end_of_r - i - 1]) {
            offset++;
            std::swap(m_col_inds[end_of_r - i], m_col_inds[end_of_r - i - 1]);
            std::swap(m_values[end_of_r - i], m_values[end_of_r - i - 1]);
        }
    }
    return m_values[end_of_r - offset];
}


Vector SparseMatrix::operator*(Vector v) const
{
    Vector b(m_rows, 0);

    for(size_t r = 0; r < m_rows; ++r){
        SparseMatrix::ConstRowIterator it = this->begin(r);
        SparseMatrix::ConstRowIterator end= this->end(r);
        for(; it != end; ++it){
            b[r] += it.value() * v[it.col_index()];
        }
    }
    return b;
}

void SparseMatrix::clear(){

    m_values = std::vector<double>(m_row_capacity * m_rows, m_zero);
    m_col_inds = std::vector<size_t>(m_row_capacity * m_rows, (size_t) - 1);

}

std::ostream& operator<<(std::ostream& stream, const SparseMatrix& m)
{
    for(size_t r = 0; r < m.num_rows(); ++r){
        for(size_t c = 0; c < m.num_cols(); ++c){
            std::cout << std::setw(6) << std::setfill(' ') << std::setprecision(2) << std::fixed << m(r,c) << "\t";
        }
        std::cout << std::endl;
    }
    return stream;
}




// explicit template instantiations
template class SparseMatrix::RowIteratorBase<true>;
template class SparseMatrix::RowIteratorBase<false>;