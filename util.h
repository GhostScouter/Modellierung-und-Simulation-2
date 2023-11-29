/*
 * util.h
 *
 *  Created on: 03.05.2017
 *      Author: mbreit
 */

#ifndef UTIL_H
#define UTIL_H


#include "vector.h"
#include "structured_grid.h"
#include "coord_vector.h"

/**
 * @brief Sample a function on a grid.
 * This function takes a pointer to a scalar-valued function in a d-dimensional space
 * and samples it on a d-dimensional structured grid. The sample values are written to
 * a vector according to the order imposed by the structured grid.
 *
 * @param gvOut  output vector containing sample values
 * @param grid   structured grid
 * @param fct    pointer to function
 */
template <std::size_t dim>
void sample_function_to_grid_vector
(
	Vector& gvOut,
	const StructuredGrid<dim>& grid,
	double (*fct)(const CoordVector<dim>&)
);


#endif // UTIL_H
