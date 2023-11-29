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

template <std::size_t dim>
VTKExporter<dim>::VTKExporter(const StructuredGrid<dim>& grid)
: m_grid(grid)
{}


template <std::size_t dim>
void VTKExporter<dim>::export_vector(const Vector& vec, const std::string& fctName, const std::string& filename)
{
	// todo: imeplement!
}



// explicit template declarations
template class VTKExporter<1>;
template class VTKExporter<2>;
template class VTKExporter<3>;
