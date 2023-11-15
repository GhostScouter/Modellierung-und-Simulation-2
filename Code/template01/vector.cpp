/*
 * vector.cpp
 *
 *  Created on: 2019-04-28
 *      Author: 
 */


#include <cassert>
#include <cmath>


Vector::Vector()
{}

Vector::Vector(std::size_t sz, double val)  // construct a vector
: std::vector<double>(sz, val)              // use the standard vector
{}

Vector::~Vector()
{}



Vector& Vector::operator=(double d)         // set new values
{
	for(auto it = this->begin(); it != this->end(); ++it){      // iterator "it" geht von Anfang bis ende den Vektor durch
        *it = d;                                                // *iterator ist der Wert an der Stelle
    }
    return *this;
}


Vector& Vector::operator+=(const Vector& v) // addiere Vektoren
{

    const std::size_t sz = this->size();    // sz ist Größe unserers Vektors
    assert(v.size() == sz);                 // prüfe auf identische Größe

    for(size_t i = 0; i < sz; ++i){         // iteriere über vektor
        this->operator[](i) += v[i];        // Wert von Vektor an Stelle i + Wert des anderen an Stelle i
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


Vector& Vector::operator*=(double d)        // Streckung eines Vektor
{
    for(auto it = this->begin(); it != this->end(); ++it){
        *it *= d;
    }
    return *this;
}


Vector Vector::operator+(const Vector& v) const         // Addition zweier Vektoren. Return new Instance
{
    const std::size_t sz = this->size();
    assert(v.size() == sz);

    Vector vOut = Vector(sz, 0.0);                      // Erstelle Ergebnisvektor

    for(size_t i = 0; i < sz; ++i){
        vOut[i] = this->operator[](i) + v[i];           // Fülle Ergebnisvektor
    }
    return vOut;                                        // return new instance
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


double Vector::operator*(const Vector& v) const         // Skalarproduct zweier Vektoren
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
	return sqrt(operator*(*this));      // sqrt des Skalarprodukts ist Norm!
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
