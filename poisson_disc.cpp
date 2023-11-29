/*
 * poisson_disc.cpp
 *
 *  Created on: 26.04.2017
 *      Author: mbreit
 */

#include "poisson_disc.h"
#include "matrix.h"
#include "sparse_matrix.h"


template <std::size_t dim, typename TMatrix>
PoissonDisc<dim, TMatrix>::PoissonDisc()
: m_diriBndFct(NULL), m_rhs(NULL)
{}


template <std::size_t dim, typename TMatrix>
void PoissonDisc<dim, TMatrix>::set_dirichlet_boundary(const DirichletBndFct fct)
{
	m_diriBndFct = fct;
}


template <std::size_t dim, typename TMatrix>
void PoissonDisc<dim, TMatrix>::set_rhs(const RhsFct fct)
{
	m_rhs = fct;
}


template <std::size_t dim, typename TMatrix>
void PoissonDisc<dim, TMatrix>::assemble
(
	matrix_type& mat,
	vector_type& rhs,
	const vector_type& u,
	const StructuredGrid<dim>& m_grid
) const
{
	// clear and resize matrix and rhs
	std::size_t nVrt = m_grid.num_vertices();
	mat.clear();
	mat.resize(nVrt, nVrt, 0.0);
	rhs.clear();
	rhs.resize(nVrt, 0.0);

	CoordVector<dim> vrtCoords, neighborCoords;
	for (std::size_t vrt = 0; vrt < nVrt; ++vrt)
	{
		m_grid.vertex_coords(vrtCoords, vrt);

		// assemble Poisson row

		// matrix entries
		std::vector<std::size_t> neighbors;
		m_grid.vertex_neighbors(neighbors, vrt);
		std::size_t nNeighbors = neighbors.size();
		for (size_t n = 0; n < nNeighbors; ++n)
		{
			std::size_t neighbor = neighbors[n];

			m_grid.vertex_coords(neighborCoords, neighbor);
			double h_sqinv = 1.0 / vrtCoords.distanceSquared(neighborCoords);
			mat(vrt, vrt) += h_sqinv;
			mat(vrt, neighbor) -= h_sqinv;
		}

		// rhs entry
		// in parallel, we must only assemble if vertex is not SLAVE
		if (m_rhs)
			rhs[vrt] = m_rhs(vrtCoords);
	}


	// on Dirichlet boundary vertices, assemble Dirichlet row (symmetrically!)
	for (std::size_t vrt = 0; vrt < nVrt; ++vrt)
	{
		m_grid.vertex_coords(vrtCoords, vrt);

		double bndVal;
		if (m_grid.is_boundary(vrt) && m_diriBndFct && m_diriBndFct(bndVal, vrtCoords))
		{
			// eliminate row and column
			std::vector<std::size_t> neighbors;
			m_grid.vertex_neighbors(neighbors, vrt);
			std::size_t nNeighbors = neighbors.size();
			for (size_t n = 0; n < nNeighbors; ++n)
			{
				std::size_t neighbor = neighbors[n];

				mat(vrt, neighbor) = 0.0;
				rhs[neighbor] = rhs[neighbor] - mat(neighbor, vrt) * bndVal;
				mat(neighbor, vrt) = 0.0;
			}

			// write a Dirichlet row
			mat(vrt, vrt) = 1.0;
			rhs[vrt] = bndVal;
		}
	}
}



template <std::size_t dim, typename TMatrix>
void PoissonDisc<dim, TMatrix>::assemble
(
	matrix_type& mat,
	const vector_type& u,
	const StructuredGrid<dim>& m_grid
) const
{
	// clear and resize matrix and rhs
	std::size_t nVrt = m_grid.num_vertices();
	mat.clear();
	mat.resize(nVrt, nVrt, 0.0);

	CoordVector<dim> vrtCoords, neighborCoords;
	for (std::size_t vrt = 0; vrt < nVrt; ++vrt)
	{
		m_grid.vertex_coords(vrtCoords, vrt);

		// matrix entries
		std::vector<std::size_t> neighbors;
		m_grid.vertex_neighbors(neighbors, vrt);
		std::size_t nNeighbors = neighbors.size();
		for (size_t n = 0; n < nNeighbors; ++n)
		{
			std::size_t neighbor = neighbors[n];

			m_grid.vertex_coords(neighborCoords, neighbor);
			double h_sqinv = 1.0 / vrtCoords.distanceSquared(neighborCoords);
			mat(vrt, vrt) += h_sqinv;
			mat(vrt, neighbor) -= h_sqinv;
		}
	}

	for (std::size_t vrt = 0; vrt < nVrt; ++vrt)
	{
		m_grid.vertex_coords(vrtCoords, vrt);

		// on Dirichlet boundary vertices, assemble Dirichlet row
		double bndVal;
		if (m_grid.is_boundary(vrt) && m_diriBndFct && m_diriBndFct(bndVal, vrtCoords))
		{

			std::vector<std::size_t> neighbors;
			m_grid.vertex_neighbors(neighbors, vrt);
			std::size_t nNeighbors = neighbors.size();
			for (size_t n = 0; n < nNeighbors; ++n)
			{
				std::size_t neighbor = neighbors[n];
				m_grid.vertex_coords(neighborCoords, neighbor);

				mat(vrt, neighbor) = 0;
				mat(neighbor, vrt) = 0;
			}

			mat(vrt, vrt) = 1.0;
		}
	}
}



// explicit template declarations
template class PoissonDisc<1, Matrix>;
template class PoissonDisc<2, Matrix>;
template class PoissonDisc<3, Matrix>;
template class PoissonDisc<1, SparseMatrix>;
template class PoissonDisc<2, SparseMatrix>;
template class PoissonDisc<3, SparseMatrix>;
