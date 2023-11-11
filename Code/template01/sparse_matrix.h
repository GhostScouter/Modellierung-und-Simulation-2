/*
 * sparse_matrix.h
 *
 *  Created on: 2019-04-28
 *      Author: mbreit
 */

#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H


#include <cstddef>
#include <vector>
#include <cassert>
#include <set>
#include <map>
#include "vector.h"

using namespace std;


class SparseMatrixIteratorRegistry;


class SparseMatrix
{
	private:

		size_t capacity;
		size_t rows;
		size_t columns;
		size_t vectorSize;
		size_t* colInds;
		double* values;
		

	public:
		/**
		 * @brief Constructor without arguments
		 * The maximal number of non-zero entries per row is set to a default value of 5.
		 */
		SparseMatrix();

		/** @brief Constructor with size, maximal number of non-zero entries per row and optional default value
		 *	The matrix is to be initialized with the given sizes and all entries are to be initialized with 0.0.
		**/
		SparseMatrix(std::size_t r, std::size_t c, std::size_t rowCapacity = 5);

		/// Destructor
		virtual ~SparseMatrix();


		/// Return number of rows
		std::size_t num_rows() const;

		/// Return number of columns
		std::size_t num_cols() const;

		SparseMatrix& printMatrix();

		/// Resize the matrix
		/**
		 * The matrix is resized to the given sizes.
		 * New elements are always initialized with 0.0, regardless of the
		 * default value parameter, which is only present to enhance compatibility.
		 */
		void resize(std::size_t r, std::size_t c, double defVal = 0.0);

	
	public:
		/// helper struct for templated iterator type
		template <bool is_const, typename dummy = void>
		struct iterator_traits
		{
			typedef SparseMatrix matrix_type;
			typedef double entry_type;
		};
		template<typename dummy>
		struct iterator_traits<true, dummy>
		{
			typedef const SparseMatrix matrix_type;
			typedef const double entry_type;
		};

		/**
		 * @brief row iterator nested class
		 * We implement const and non-const version at the same time
		 * using a bool template parameter.
		 */
		template <bool is_const>
		class RowIteratorBase
		{
			private:
				size_t vectorPosition;
				size_t fakeColumnPosition;

			public:
				/// Constructor with row; set to first col
				RowIteratorBase(typename iterator_traits<is_const>::matrix_type& mat, std::size_t rowIndex);

				/**
				 * @brief Constructor with row and col index
				 * Sets iterator to first column with index greater than or equal to startFromCol.
				**/
				RowIteratorBase(typename iterator_traits<is_const>::matrix_type& mat, std::size_t rowIndex, std::size_t startFromCol);

				/**
				 * @brief Compares iterator with given iterator
				 * @return true if iterators point to different positions
				 */
				bool operator!=(RowIteratorBase& other) const;
				
				/**
				 * @brief Compares iterator with given iterator
				 * @return true if iterators point to same position
				 */
				bool operator==(RowIteratorBase& other) const;

				/// increment iterator to next position
				RowIteratorBase& operator++();

				/// value the iterator points to (read-only)
				typename iterator_traits<is_const>::entry_type value() const;

				/// value the iterator points to
				typename iterator_traits<is_const>::entry_type& value();

				/// column the iterator points to
				std::size_t col_index() const;

				friend class SparseMatrix;

			private:
				typename iterator_traits<is_const>::entry_type* pCurVal;
				const std::size_t* pCurInd;
		};

		typedef RowIteratorBase<false> RowIterator;
		typedef RowIteratorBase<true> ConstRowIterator;


	public:
		/// iterator to first column entry of row
		RowIterator begin(std::size_t r);

		/// iterator to behind last column entry of row
		RowIterator end(std::size_t r);

		/// iterator to first column entry of row
		ConstRowIterator begin(std::size_t r) const;

		/// iterator to behind last column entry of row
		ConstRowIterator end(std::size_t r) const;

		/// Return whether entry exists
		bool has_entry(std::size_t r, std::size_t c) const;

		/**
		 * @brief Element access (read-only)
		 * Returns the value at the given position.
		 */
		double operator()(std::size_t r, std::size_t c) const;

		/**
		 * @brief Element access
		 * Returns a reference to the value at the given position.
		 * If the entry is not in the matrix, it is created.
		 * In that process, it may happen that the row is already full.
		 * This has to produce an error (or - advanced - the row capacity has to be increased).
		 */
		double& operator()(std::size_t r, std::size_t c);


		/**
		 * @brief Matrix-vector multiplication
		 * The matrix is applied on the given vector.
		 * The result is returned in a new instance.
		 */
		Vector operator*(Vector v) const;
};



/// write matrix to output stream
std::ostream& operator<<(std::ostream& stream, const SparseMatrix& m);


#endif // SPARSE_MATRIX_H