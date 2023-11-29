/*
 * vector.h
 *
 *  Created on: 2019-04-28
 *      Author: mbreit
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <ostream>

class Vector: public std::vector<double>

{

	public:
    double* vector; // Member variable to hold the array
    size_t vectorSize; // Member variable to hold the size of the array
	double vectorFilling; // Member variable with the value that the array contains at all positions

	public:

		/** @brief Constructor without arguments
		 *	The vector is to be initialized with a size of 0.
		**/
		Vector();

		/** @brief Constructor with size and optional default value
		 *	The vector is to be initialized with the given size and all elements
		 *	are to be initialized with the given default value (0.0 if not given).
		**/
		Vector(std::size_t sz, double val = 0.0);

		/// @brief Destructor
		virtual ~Vector();

		/// Set a constant value to each component of the vector
		Vector& operator=(double d);

		Vector& printVector();

		/// Add a vector to this vector and return a self-reference
		Vector& operator+=(const Vector& v);

		///  Subtract a vector from this vector and return a self-reference
		Vector& operator-=(const Vector& v);

		///  Multiply this vector by a scalar and return a self-reference
		Vector& operator*=(double d);



		/// Add this vector and another vector and return in new instance
		Vector operator+(const Vector& v) const;

		/// Subtract another vector from this vector and return in new instance
		Vector operator-(const Vector& v) const;

		/// Multiply this vector by a scalar and return in new instance
		Vector operator*(double d) const;

		/// Scalar product of two vectors (of the same size)
		double operator*(const Vector& v) const;

		/// Euclidean norm
		double norm();

    	// Declare the size() member function
    	std::size_t size() const;
};

//std::ostream& operator<<(std::ostream& stream, const Vector& v);

#endif // VECTOR_H