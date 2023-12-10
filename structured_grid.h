

#ifndef STRUCTURED_GRID_H
#define STRUCTURED_GRID_H

#include "coord_vector.h"
#include "vector.h"

#include <cstddef>
#include <vector>


/**
 * Structured grid.
 * This class represents a structured grid that is equi-distant w.r.t.
 * all dimensions separately. It is defined by the lower and upper bounds
 * for all dimensions as well as the spatial resolution (i.e., number of vertices)
 * in all dimensions. Vertices are connected only by axis-parallel edges.
 */
template <std::size_t dim>
class StructuredGrid
{
	public:
		StructuredGrid() {};

		/** @brief Constructor with bounds and resolution
		 * Constructs the grid with volume (a1,b1) [x(a2,b2) [x(a3,b3)]]
		 * with the given lower bounds a1 [, a2, [a3]], the given upper bounds
		 * b1 [, b2, [b3]] and the number of vertices (resolution) in each dimension.
		 *
		 * Example: StructuredGrid({0,0}, {1,2}, {3,3}) would create a grid
		 * with vertices in (0,0), (0.5,0), (1,0); (0,1), (0.5,1), (1,1); and
		 * (0,2), (0.5,2), (1,2).
		 */
		StructuredGrid
		(
			const CoordVector<dim>& lowerBounds,
			const CoordVector<dim>& upperBounds,
			const CoordVector<dim, std::size_t>& resolution
		);


		/// Number of vertices in the grid
		std::size_t num_vertices() const;

		/** @brief Coordinates for vertex index
		 * Returns the coordinates for a vertex index for an implementation-specified
		 * order of vertices.
		 */
		void vertex_coords(CoordVector<dim>& coordsOut, std::size_t ind) const;
		
		/** @brief Returns index corresponding to multi_index numbering
		 *  E.g., on a 5x5x5 vertices grid with lexicographic ordering, the index
		 *  for the vertex located at vertex (3,0,1) in the grid is 3*25 + 0*5 + 1 = 76.
		 */
		std::size_t index(const CoordVector<dim, std::size_t>& vMultiIndex) const;

		/** @brief returns multi-index in lexicographic numbering
		 *  This is the inverse mapping of the index() method.
		 */
		CoordVector<dim, std::size_t> multi_index(std::size_t ind) const;

		/** @brief Vertex neighbors
		 * Returns the neighboring vertex indices for any vertex index.
		 * A neighbor is a vertex that is directly adjacent
		 * in one of the coordinate axis directions.
		 */
		void vertex_neighbors(std::vector<std::size_t>& neighborsOut, std::size_t ind) const;

		/// Whether a vertex index belongs to a vertex on the grid boundary
		bool is_boundary(std::size_t ind) const;

		/// Vector of lower grid bounds
		const CoordVector<dim>& lower_bnds() const;

		/// Vector of upper grid bounds
		const CoordVector<dim>& upper_bnds() const;

		/// Number of vertices (resolution) for each dimension
		const CoordVector<dim, std::size_t>& mesh_sizes() const;

		/// Mesh size (distance of adjacent vertices) in each dimension
		const CoordVector<dim>& element_sizes() const;


	protected:
		CoordVector<dim> m_lowerBnds;
		CoordVector<dim> m_upperBnds;
		CoordVector<dim, std::size_t> m_resolution;
		CoordVector<dim> m_h;
};


#endif // STRUCTURED_GRID_H
