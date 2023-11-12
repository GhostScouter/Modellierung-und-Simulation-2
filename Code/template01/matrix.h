/*
 * matrix.h
 *
 *  Created on: 2019-04-28
 *      Author: mbreit
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>


class Matrix
{
	private:
		size_t m_rows;
		size_t m_cols;
		Vector m_mat;

	public:
		/** @brief Constructor without arguments
		 *	The matrix is to be initialized with a size of 0x0.
		**/
		Matrix();

		/** @brief Constructor with size and optional default value
		 *	The matrix is to be initialized with the given sizes and all elements
		 *	are to be initialized with the given default value (0.0 if not given).
		**/
		Matrix(std::size_t r, std::size_t c, double val = 0.0);

		Matrix(const Matrix&);

		/// Destructor
		virtual ~Matrix();

		/// Return number of rows
		std::size_t num_rows() const;

		/// Return number of columns
		std::size_t num_cols() const;

		/// Resize the matrix
		/**
		 * The matrix is resized to the given sizes. If elements are created
		 * in that process, they are to be initialized with the optional default
		 * value (0.0 if not given).
		 */
		void resize(std::size_t r, std::size_t c, double val = 0.0);

		Matrix& printMatrix();

	public:
		/// helper struct for templated iterator type
		template <bool is_const, typename dummy = void>
		struct iterator_traits
		{
			typedef Matrix matrix_type;
			typedef double entry_type;
		};
		template<typename dummy>
		struct iterator_traits<true, dummy>
		{
			typedef const Matrix matrix_type;
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
			public:

			    typename std::conditional<is_const, const Matrix*, Matrix*>::type pMat;
				size_t pRowIdx;
				size_t pColIdx;

			public:
				/// Constructor with row; set to first col
				RowIteratorBase(typename iterator_traits<is_const>::matrix_type& mat, std::size_t rowIndex);

				/**
				 * @brief Constructor with row and col index
				 * Sets iterator to first column with index greater than startFromCol.
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

				/// (pre-)increment iterator to next position
				RowIteratorBase& operator++();
				
				/// value the iterator points to (read-only)
				typename iterator_traits<is_const>::entry_type value() const;

				/// value the iterator points to
				typename iterator_traits<is_const>::entry_type value();

				/// column the iterator points to
				std::size_t col_index() const;
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
		
		
		/**
		 * @brief Element access (read-only)
		 * Returns the value at the given position.
		 */
		double operator()(std::size_t r, std::size_t c) const;

		/**
		 * @brief Element access
		 * Returns a reference to the value at the given position.
		 */
		double& operator()(std::size_t r, std::size_t c);


		/**
		 * @brief Matrix-vector multiplication
		 * The matrix is applied on the given vector.
		 * The result is returned in a new instance.
		 */
		Vector operator*(Vector v) const;

		void clear();

};


#endif // MATRIX_H
