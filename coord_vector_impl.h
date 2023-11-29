

#include "coord_vector.h"

#include <cassert>

template <std::size_t dim, typename T>
CoordVector<dim, T>::CoordVector()
{
	for (std::size_t i = 0; i < dim; ++i)  // für jede Dimension wird ein Eintrag angelegt, um bspw. pro Dimension den kleinsten oder größten Eintrag zu speichern
		m_v[i] = 0.0;
}

template <std::size_t dim, typename T>
CoordVector<dim, T>::CoordVector(const CoordVector<dim, T>& v)
{
	for (std::size_t i = 0; i < dim; ++i)
		m_v[i] = v.m_v[i];
}

template <std::size_t dim, typename T>
CoordVector<dim, T>& CoordVector<dim, T>::operator=(const CoordVector<dim, T>& v)
{
    for (std::size_t i = 0; i < dim; ++i)
        m_v[i] = v.m_v[i];
    return *this;
}

template <std::size_t dim, typename T>
CoordVector<dim, T> CoordVector<dim, T>::operator*(const T& s) const
{
	CoordVector<dim, T> res;
    for (std::size_t i = 0; i < dim; ++i)
        res[i] = m_v[i] * s;
    return res;
}

template <std::size_t dim, typename T>
CoordVector<dim, T>::CoordVector(T d)
{
	for (std::size_t i = 0; i < dim; ++i)
		m_v[i] = d;
}

template <std::size_t dim, typename T>
CoordVector<dim, T>::CoordVector(const std::vector<T>& v)
{
	assert(v.size() == dim && "Size mismatch in coordinate vector constructor.");

	for (std::size_t i = 0; i < dim; ++i)
		m_v[i] = v[i];
}

template <std::size_t dim, typename T>
const T& CoordVector<dim, T>::operator[](std::size_t i) const
{
	assert(i < dim && "Tried to access invalid coordinate vector index.");
	return m_v[i];
}

template <std::size_t dim, typename T>
T& CoordVector<dim, T>::operator[](std::size_t i)
{
	assert(i < dim && "Tried to access invalid coordinate vector index.");
	return m_v[i];
}


template <std::size_t dim, typename T>
T CoordVector<dim, T>::distanceSquared(const CoordVector<dim, T>& v)
{
	T res = 0;
	for (std::size_t i = 0; i < dim; ++i)
	{
		T dimDist = m_v[i] - v.m_v[i];
		res += dimDist * dimDist;
	}

	return res;
}


template<std::size_t dim, typename T>
std::ostream& operator<<(std::ostream& stream, const CoordVector<dim, T>& v)
{
	stream << "(";
	for (std::size_t i = 0; i < dim-1; ++i)
		stream << v[i] << " ";
	stream << v[dim-1] << ")";

	return stream;
}
