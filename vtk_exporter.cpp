/*
 * vtk_exporter.cpp
 *
 *  Created on: 26.04.2017
 *      Author: mbreit
 */

#include "vtk_exporter.h"
#include "coord_vector.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iostream>

template <std::size_t dim>
VTKExporter<dim>::VTKExporter(const StructuredGrid<dim>& grid)
: m_grid(grid)
{}


template <std::size_t dim>
void VTKExporter<dim>::export_vector(const Vector& vec, const std::string& fctName, const std::string& filename)
{
	// OFFENE FRAGEN: Was soll die Variable "fctName"?

	// Diese Funktion bekommt einen Vektor gegeben und soll diesen exportieren
	// Erstelle Dokument

	std::ofstream outFile (filename);

	if (outFile.is_open()){
		std::cout "File created successfully." << std::endl;


	// Schreibe "Header" hinein

	outFile << "# vtk DataFile Version 2.0 " << std::endl;
	outFile << "Volume example" << std::endl;
	outFile << "ASCII" << std::endl;
	outFile << "DATASET STRUCTURED_POINTS" << std::endl;

	// Wir geben nun Dimensions aus.
	outFile << "DIMENSIONS";
	
	for(size_t i = 0; i < dim; i++){
		outFile << " " << m_grid.m_resolution[i];
	}
	outFile << std::endl;

	// Als nächstes geben wir "SPACING" aus. Vermutlich handelt es sich um die Schrittweiten? 
	outFile << "SPACING";
	
	for(size_t i = 0; i < dim; i++){
		outFile << " " << ((m_grid.m_upperBnds[i]-m_grid.m_lowerBnds[i])/m_grid.m_resolution[i]);
	}
	outFile << std::endl;

	// Als nächstes geben wir "ORIGIN" aus.
	outFile << "ORIGIN";
	
	for(size_t i = 0; i < dim; i++){
		outFile << " " << m_grid.m_lowerBnds[i];
	}
	outFile << std::endl;

	// POINT DATA
	outFile << "POINT DATA";
	size_t result = 1;
	for(size_t i = 0; i < dim; i++){
		result *= m_grid.m_resolution[i];
	} 
	outFile << result << std::endl;

	// Was macht das?
	outFile << "SCALARS volume_scalars double 1" << std::endl;
	outFile << "LOOKUP_TABLE default" << std::endl;
	

	// Schreibe nun die Werte der Punkte
	for(size_t i = 0; i < vec.size(); i++){
		outFile << vec[i] << std::endl;
	}
	outFile.close();

	}// Falls File nicht erstellt werden konnte:
	else{
		std::cerr << "Error creating File." << std::endl;
	}
	
	return 0;
}


// explicit template declarations
template class VTKExporter<1>;
template class VTKExporter<2>;
template class VTKExporter<3>;
