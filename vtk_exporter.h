/*
 * vtk_exporter.h
 *
 *  Created on: 26.04.2017
 *      Author: mbreit
 */

#ifndef VTK_EXPORTER_H
#define VTK_EXPORTER_H

#include "structured_grid.h"
#include "vector.h"
#include <string>


template <std::size_t dim>
class VTKExporter
{
	public:
		/** @brief Constructor with grid
		 * Constructs an exporter object that will use the specified grid
		 * to calculate positions for the indices of any vector to be exported.
		 */
		VTKExporter(const StructuredGrid<dim>& grid);

		/** \brief Export vector
		 * The vector values are exported using the position assign to their
		 * associated indices by the grid.
		 *
		 * @param vec        vector to be exported
		 * @param fctName    name for the data
		 * @param filename   name for the output file (*.vtk)
		 */
		void export_vector(const Vector& vec, const std::string& fctName, const std::string& filename);

	private:
		const StructuredGrid<dim>& m_grid;
};

#endif // VTK_EXPORTER_H
