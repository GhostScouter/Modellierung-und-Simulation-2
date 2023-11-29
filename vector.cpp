/*
 * vector.cpp
 *
 *  Created on: 2019-04-28
 *      Author: 
 */

#include "vector.h"
#include <cassert>
#include <cmath>


Vector::Vector()
{}

Vector::Vector(std::size_t sz, double val)
: std::vector<double>(sz, val)
{
}

Vector::~Vector()
{}



Vector& Vector::operator=(double d)
{
	for(auto it = this->begin(); it != this->end(); ++it){
        *it = d;
    }
    return *this;
}


Vector& Vector::operator+=(const Vector& v)
{
    assert(v.size() == this->size());
    const std::size_t sz = this->size();
    for(size_t i = 0; i < sz; ++i){
        this->operator[](i) += v[i];
    }
    return *this;
}

Vector& Vector::operator-=(const Vector& v)
{
    assert(v.size() == this->size());
    const std::size_t sz = this->size();
    for(size_t i = 0; i < sz; ++i){
        this->operator[](i) -= v[i];
    }
    return *this;
}


Vector& Vector::operator*=(double d)
{
    for(auto it = this->begin(); it != this->end(); ++it){
        *it *= d;
    }
    return *this;
}


Vector Vector::operator+(const Vector& v) const
{
    const std::size_t sz = this->size();
    assert(v.size() == sz);
    Vector vOut = Vector(sz, 0.0);
    for(size_t i = 0; i < sz; ++i){
        vOut[i] = this->operator[](i) + v[i];
    }
    return vOut;
}


Vector Vector::operator-(const Vector& v) const
{
    const std::size_t sz = this->size();
    assert(v.size() == sz);
    Vector vOut = Vector(sz, 0.0);
    for(size_t i = 0; i < sz; ++i){
        vOut[i] = this->operator[](i) - v[i];
    }
    return vOut;
}


Vector Vector::operator*(double d) const
{
    const std::size_t sz = this->size();
    Vector vOut = Vector(sz, 0.0);
    for(size_t i = 0; i < sz; ++i){
        vOut[i] = this->operator[](i) * d;
    }
    return vOut;
}


double Vector::operator*(const Vector& v) const
{
    const std::size_t sz = this->size();
    assert(v.size() == sz);
    double result = 0;
    for(size_t i = 0; i < sz; ++i){
        result += this->operator[](i) * v[i];
    }
    return result;
}


double Vector::norm()
{
	return sqrt(operator*(*this));
}


std::ostream& operator<<(std::ostream& stream, const Vector& v)
{
	if (v.empty()) return stream << "()";

	std::size_t sz = v.size() - 1;
	stream << "(";
	for (std::size_t i = 0; i < sz; ++i)
		stream << v[i] << " ";
	stream << v[sz] << ")";

	return stream;
}

