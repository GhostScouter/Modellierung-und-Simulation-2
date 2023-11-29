/*
 * poisson_disc.h
 *
 *  Created on: 26.04.2017
 *      Author: mbreit
 */

#ifndef POISSON_DISC_H
#define POISSON_DISC_H

#include "assemble_interface.h"
#include "coord_vector.h"
#include "structured_grid.h"
#include "vector.h"
#include <cstddef>



/** @brief Finite differences discretization for the Poisson equation.
 * This class assembles matrix and vector for a finite differences
 * discretization of the Poisson equation on a structured grid.
 *
 * The Poisson equation reads:
 *     -div grad u = f,
 * where u is the unknown function and f is a known function ("rhs").
 * A Dirichlet condition,
 *     u = g on some part of the boundary,
 * where d is a known function, allows for a unique solution to this problem.
 *
 * Both right-hand side (f) and Dirichlet value (g) are provided to this
 * implementation via function pointers.
 */
template <std::size_t dim, typename TMatrix>
class PoissonDisc : public IAssemble<dim, TMatrix>
{
	public:
		/// Type declaration for the Dirichlet boundary function pointer type
		typedef bool (*DirichletBndFct)(double& value, const CoordVector<dim>& coords);

		/// Type declaration for the right-hand side function pointer type
		typedef double (*RhsFct)(const CoordVector<dim>& coords);

		typedef TMatrix matrix_type;
		typedef Vector vector_type;

	public:
		/// Constructor
		PoissonDisc();

		/** @brief Set the Dirichlet boundary function
		 * The function is passed as a pointer: If you have defined a function
		 * "diri" with the correct signature, simply call this method with
		 * set_dirichlet_boundary(&diri);
		 */
		void set_dirichlet_boundary(const DirichletBndFct fct);

		/** @brief Set the right-hand side function
		 * The function is passed as a pointer: If you have defined a function
		 * "rhs" with the correct signature, simply call this method with
		 * set_rhs(&rhs);
		 */
		void set_rhs(const RhsFct fct);

		/** @brief Assemble matrix and rhs vector for the discretization
		 * The matrix and vector are resized according to the number of
		 * vertices contained in the underlying grid.
		 */
		void assemble
		(
			matrix_type& mat,
			vector_type& rhs,
			const vector_type& u,
			const StructuredGrid<dim>& grid
		) const;

		/** @brief Assemble matrix for the discretization
		 * The matrix is resized according to the number of
		 * vertices contained in the underlying grid.
		 */
		void assemble
		(
			matrix_type& mat,
			const vector_type& u,
			const StructuredGrid<dim>& grid
		) const;


	private:
		DirichletBndFct m_diriBndFct;
		RhsFct m_rhs;
};


#endif // POISSON_DISC_H
