/*
 * vector.cpp
 *
 *  Created on: 2019-04-28
 *      Author: 
 */

#include "vector.h"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;

Vector::Vector()
{
    vectorSize = 0;
    vector = new double[vectorSize];
	vectorFilling = 0;

	for (int i = 0; i < vectorSize; ++i) {
        vector[i] = vectorFilling;
	}		
}

Vector::Vector(std::size_t sz, double val)
{
    vectorSize = sz;
    vector = new double[vectorSize];
	vectorFilling = val;

	for (int i = 0; i < vectorSize; ++i) {
            vector[i] = vectorFilling;
	}
}

Vector::~Vector()
{
    delete[] vector;
    }


Vector& Vector::printVector() {
	std::cout << vector << std::endl;
	for (int i = 0; i < vectorSize; ++i) {
		std::cout << vector[i] << ' ';
	}
	std::cout << std::endl;
}

Vector& Vector::operator=(double d)
{
        for (int i = 0; i < vectorSize; ++i) {
            vector[i] = d;
        }
}

Vector& Vector::operator+=(const Vector& v)
{
        for (int i = 0; i < vectorSize; ++i) {
            vector[i] = vector[i] + v.vectorFilling;
        }
        return *this; // Return a reference to the current instance
    }


Vector& Vector::operator-=(const Vector& v)
{
		for (int i = 0; i < vectorSize; ++i) {
            vector[i] = vector[i] - v.vectorFilling;
        }
        return *this; // Return a reference to the current instance
    }


Vector& Vector::operator*=(double d)
{
        for (int i = 0; i < vectorSize; ++i) {
            vector[i] = d * vector[i];
        }
        return *this; // Return a reference to the current instance
    }


Vector Vector::operator+(const Vector& v) const
{
    double newFilling = vectorFilling + v.vectorFilling;
	cout << newFilling << endl;
	Vector v(vectorSize, 0);
    return Vector (vectorSize, newFilling); // Return a new instance of vector
}


Vector Vector::operator-(const Vector& v) const
{
    double newFilling = vectorFilling - v.vectorFilling;
	cout << newFilling << endl;
    return Vector (vectorSize, newFilling); // Return a new instance of vector
}


Vector Vector::operator*(double d) const
{
    double newFilling = vectorFilling * d;
	cout << newFilling << endl;
    return Vector (vectorSize, newFilling); // Return a new instance of vector
}


double Vector::operator*(const Vector& v) const
{
        double scalarProduct = 0;
		for (int i = 0; i < vectorSize; ++i) {
            scalarProduct = scalarProduct + vectorFilling * v.vectorFilling;
        }
        return scalarProduct; // Return a reference to the current instance
    }
    

double Vector::norm()
{
        double norm = sqrt(vectorSize * vectorFilling * vectorFilling); 
        return norm; // Return a reference to the current instance
    }

std::size_t Vector::size() const {
    return vectorSize;
}