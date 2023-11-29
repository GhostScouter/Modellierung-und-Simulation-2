/*
 * iterative_solver.h
 *
 *  Created on: 2019-05-03
 *      Author: mbreit
 */

#ifndef ITERATIVE_SOLVER_H
#define ITERATIVE_SOLVER_H

#include "vtk_exporter.h"
#include "vector.h"
#include "preconditioner_interface.h"
#include "linear_solver_interface.h"
#include <cstddef>
#include <memory>


/** @brief An iterative linear solver
 * This solver iterates the following process for a given matrix A and rhs b:
 *     d_i     = A * x_i - b   ("defect")
 *     c_i     = M^{-1} * d_i  ("correction")
 *     x_{i+1} = x_i - c_i     ("solution")
 * with an iteration matrix M that is an approximation of A and typically easy to invert.
 * The inversion (and implicit definition) of M is provided by a "corrector" method that
 * has to be assigned to any instance of this class. It has to implement the IPreconditioner
 * interface. Examples for such "correctors" are Jacobi and GaussSeidel.
 */
template <typename TMatrix, size_t dim = 2>
class IterativeSolver
 : public ILinearSolver<TMatrix>
{
	public:
        typedef Vector vector_type;
		typedef TMatrix matrix_type;
		typedef IPreconditioner<TMatrix> corrector_type;

	public:
		/// constructor
		IterativeSolver();

		/// constructor with matrix
		IterativeSolver(const matrix_type& mat);

		/// set corrector method
		void set_corrector(corrector_type* corrector);

		/** @brief Set parameters to define convergence (or lack thereof)
		 * The iteration process is carried out until either of the follwing three criteria is met:
		 *   (i)   The defect norm ||d_i|| is lower than a specified minimal defect value (minDef),
		 *   (ii)  the defect norm ||d_i|| is lower than a specified factor (minRed)
		 *         times the initial defect d_0,
		 *   (iii) the number of iterations exceeds a specified value (nIter).
		 * The iteration is regarded to have converged if (i) or (ii) are the case.
		 *
		 * @param nIter   maximal number of iterations
		 * @param minDef  absolute defect norm value to be reached
		 * @param minRed  defect norm reduction factor to be reached
		 */
		void set_convergence_params(std::size_t nIter, double minDef, double minRed);

		/** @brief Initialize
		 *  If a corrector is already given, then also initialize that.
		 *  @return  true on success, false otherwise
		 */
		virtual bool init(const vector_type& x);

		/// set matrix
		virtual void set_matrix(const matrix_type* mat);

		/** @brief Perform iterative process
		 * Iteration solves A*x = b for x.
		 *
		 * @param x  output: solution
		 * @param b  input: right-hand side
		 * @return   false on any failure; true otherwise
		 */
		virtual bool solve(vector_type& x, const vector_type& b) const;
        void set_verbose(bool verbose){m_bVerbose = verbose;}
        void set_printDelta(bool printDef){m_printDelta = printDef;}
        void set_vtk(VTKExporter<dim>* sp_vtk){m_sp_vtk = sp_vtk;}
	private:
		corrector_type* m_corrector;
        const TMatrix* m_mat;
		std::size_t m_nit;
		double m_minDef;
		double m_minRed;

		bool m_bVerbose;
		bool m_bInited;
        bool m_printDelta;


        VTKExporter<dim> *m_sp_vtk;

};

//put this code snippet somewhere in the solve function!!
size_t defprint = 0
/*
   ...
   ...
   ...
*/
if(m_printDelta && (iteration_count % 5) == 0){

            if(m_sp_vtk == nullptr){
                std::cout << "no vtk output set." << std::endl;
            }
            
            std::stringstream ss;
            ss << "delta_def_analytic" << defprint << ".vtk";
            m_sp_vtk->export_vector(x, "delta_u", ss.str());
            defprint++;
        }

#endif // ITERATIVE_SOLVER_H
