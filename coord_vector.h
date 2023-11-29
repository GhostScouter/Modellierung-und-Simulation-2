

#ifndef COORD_VECTOR_H
#define COORD_VECTOR_H

#include <cstddef>
#include <vector>
#include <ostream>

/// A vector class of fixed size for efficient coordinate storage.
template<std::size_t dim, typename T = double>
class CoordVector
{
	public:
        /// default constructor initializing to zero
		CoordVector();

        /// constructor initializing to given value
        CoordVector(T d);

        /// copy constructor
		CoordVector(const CoordVector<dim, T>& v);
    
        /// assignment
        CoordVector& operator=(const CoordVector<dim, T>& v);
    
        /// scaling
        CoordVector operator*(const T& s) const;

        /// constructor with std::vector
		CoordVector(const std::vector<T>& v);

        /// const-access to element i
		const T& operator[](std::size_t i) const;

        /// non-const-access to element i
        T& operator[](std::size_t i);

        /// returns the squared distance
		T distanceSquared(const CoordVector<dim, T>& v);

	private:
        /// member data
		T m_v[dim];
};

template<std::size_t dim, typename T>
std::ostream& operator<<(std::ostream& stream, const CoordVector<dim, T>& v);

#include "coord_vector_impl.h"

#endif // COORD_VECTOR_H
