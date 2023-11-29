

#ifndef ASSEMBLE_INTERFACE_H
#define ASSEMBLE_INTERFACE_H

#include <cstddef>
#include <iostream>
#include "vector.h"
#include "matrix.h"
#include "structured_grid.h"


/** @brief Interface class for discretizations
 * Each class (PoissonDisc etc.) implementing this interface has to
 * provide the methods assemble() which will assemble matrix and/or right-hand side
 * for a given grid.
 */
template <std::size_t dim, typename TMatrix>
class IAssemble
{
	typedef Vector vector_type;
	typedef TMatrix matrix_type;

	public:
		/// constructor
		IAssemble(){};

		/// destructor
		virtual ~IAssemble() {};

		/** @brief Assemble matrix and right-hand side
		 * This method assembles the matrix and right-hand side for a discretization
		 * given a structured grid. The matrix and vector sizes are adjusted to the
		 * number of degrees of freedom in the grid, and matrix and vector are filled
		 * with an appropriate discretization of the partial differential equation.
		 *
		 * @param mat    matrix to be filled
		 * @param rhs    vector to be filled
		 * @param u      solution to be used
		 * @param grid   grid used for assembling
		 */
		virtual void assemble
		(
			matrix_type& mat,
			vector_type& rhs,
			const vector_type& u,
			const StructuredGrid<dim>& grid
		) const = 0;


		/** @brief Assemble matrix
		 * This method assembles the matrix for a discretization
		 * given a structured grid. The matrix size is adjusted to the
		 * number of degrees of freedom in the grid, and the matrix is filled
		 * with an appropriate discretization of the partial differential equation.
		 *
		 * @param mat    matrix to be filled
		 * @param u      solution to be used
		 * @param grid   grid used for assembling
		 */
		virtual void assemble
		(
			matrix_type& mat,
			const vector_type& u,
			const StructuredGrid<dim>& grid
		) const = 0;


		/** @brief Assemble Jacobian
		 * This method assembles the Jacobian for a (nonlinear) discretization
		 * given a structured grid. The vector size is adjusted to the
		 * number of degrees of freedom in the grid, and the vector is filled
		 * with an appropriate discretization for the partial differential equation.
		 *
		 * @param rhs    vector to be filled
		 * @param u      solution to be used
		 * @param grid   grid used for assembling
		 */
		virtual void assemble_jacobian
		(
			matrix_type& J,
			const vector_type& u,
			const StructuredGrid<dim>& grid
		) const
		{
			std::cout << "Jacobian assembling is not available for this discretization." << std::endl;
			throw 1;
		}


		/** @brief Assemble defect
		 * This method assembles the defect for a (nonlinear) discretization
		 * given a structured grid. The vector size is adjusted to the
		 * number of degrees of freedom in the grid, and the vector is filled
		 * with an appropriate discretization for the partial differential equation.
		 *
		 * @param rhs    vector to be filled
		 * @param u      solution to be used
		 * @param grid   grid used for assembling
		 */
		virtual void assemble_defect
		(
			vector_type& d,
			const vector_type& u,
			const StructuredGrid<dim>& grid
		) const
		{
			std::cout << "Defect assembling is not available for this discretization." << std::endl;
			throw 1;
		}


		/** @brief Reassemble matrix and right-hand side upon correction of a single
		 * component of the solution. This can usually be done with much less effort
		 * than re-assembling the whole matrix and rhs.
		 * This comes in handy when using non-linear smoothers in a non-linear GMG.
		 *
		 * @param J      Jacobian to be changed
		 * @param d      defect vector to be filled
		 * @param u_new  new solution vector
		 * @param ind    solution component that has changed
		 * @param u_old  old value of the solution in that component
		 * @param grid   grid used for assembling
		 */
		virtual void reassemble
		(
			matrix_type& J,
			vector_type& d,
			const vector_type& u_new,
			std::size_t ind,
			double u_old,
			const StructuredGrid<dim>& grid
		) const
		{
			std::cout << "Single solution component change reassembling functionality "
				<< "is not available for this discretization." << std::endl;
			throw 1;
		};
};


#endif // ASSEMBLE_INTERFACE_H
