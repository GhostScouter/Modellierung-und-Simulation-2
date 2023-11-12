#include <iostream>
#include <cmath>

class MyArrayClass {
public:

    // Constructor that takes no parameters and constructs a vector of size 0
    MyArrayClass() {
        arraySize = 0;
        array = new double[arraySize];
		arrayFilling = 0;

		for (int i = 0; i < arraySize; ++i) {
            array[i] = arrayFilling;
		}		
    }

    // Constructor that takes the size of the array as a parameter and optional content
    MyArrayClass(int size, double value = 0.0) {
        arraySize = size;
        array = new double[arraySize];
		arrayFilling = value;

		for (int i = 0; i < arraySize; ++i) {
            array[i] = arrayFilling;
		}		
    }

    // Destructor to free the allocated memory
    ~MyArrayClass() {
        delete[] array;
    }


// ################################################ METHODS ################################################ //
	// Method to print the values of a vector
    void printArray() {
        std::cout << array << std::endl;
        for (int i = 0; i < arraySize; ++i) {
            std::cout << array[i] << ' ';
        }
        std::cout << std::endl;
    }

	// Method to set a constant value to each component of the vector
    void redoArray(double value) {
        for (int i = 0; i < arraySize; ++i) {
            array[i] = value;
        }
    }

	// Method to multiply the array by a scalar and return self-reference
	MyArrayClass multiplyByScalar(double scalar) {
        for (int i = 0; i < arraySize; ++i) {
            array[i] = scalar * array[i];
        }
        return *this; // Return a reference to the current instance
    }

    // Method to multiply the array by a scalar and return a new instance
    MyArrayClass multiplyByScalar_newInstance(double scalar) {
		double newFilling = arrayFilling * scalar;
		return MyArrayClass(arraySize, newFilling);
    }

    // Method to add 2 vectors and return a self-reference
    MyArrayClass vectorAddition(MyArrayClass vectorToAdd) {
        for (int i = 0; i < arraySize; ++i) {
            array[i] = array[i] + vectorToAdd.arrayFilling;
        }
        return *this; // Return a reference to the current instance
    }

    // Method to substract one vector from another one and return a self-reference
    MyArrayClass vectorSubstraction(MyArrayClass vectorToSubstract) {
		for (int i = 0; i < arraySize; ++i) {
            array[i] = array[i] - vectorToSubstract.arrayFilling;
        }
        return *this; // Return a reference to the current instance
    }

    // Method to add 2 vectors and return a new instance
    MyArrayClass vectorAdditionNewInstance(MyArrayClass vectorToAdd) {
        double newFilling = arrayFilling + vectorToAdd.arrayFilling;
        return MyArrayClass(arraySize, newFilling); // Return a reference to the current instance
    }

    // Method to substract vector from another one and return a new instance
    MyArrayClass vectorSubstractionNewInstance(MyArrayClass vectorToSubstract) {
        double newFilling = arrayFilling - vectorToSubstract.arrayFilling;
        return MyArrayClass(arraySize, newFilling); // Return a reference to the current instance
    }

    // Method to compute the euclidian norm of a vecotr and return it
    double euclidNorm() {
        double norm = sqrt(arraySize * arrayFilling * arrayFilling); 
        return norm; // Return a reference to the current instance
    }

    // Method for scalar multiplication of 2 vector
    double scalarMultiplication(MyArrayClass vectorToMultiply) {
        double newFilling = 0;
		for (int i = 0; i < arraySize; ++i) {
            newFilling = newFilling + arrayFilling * vectorToMultiply.arrayFilling;
        }
        return newFilling; // Return a reference to the current instance
    }
    

// ################################################ Variables ################################################ //
private:
    double* array; // Member variable to hold the array
    int arraySize; // Member variable to hold the size of the array
	double arrayFilling; // Member variable with the value that the array contains at all positions

};


int main() {

	MyArrayClass firstArray(5);
    MyArrayClass secondArray(5, 2.5);
    secondArray.printArray();

    MyArrayClass test = secondArray.multiplyByScalar_newInstance(2);
    test.printArray();
    secondArray.printArray();

    return 0;
}