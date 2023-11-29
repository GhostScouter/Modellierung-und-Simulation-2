

#include "structured_grid.h"
#include "coord_vector.h"
#include "vector.h"
#include <cmath>

#include <stdexcept>  // for std::runtime_error


template <std::size_t dim>
StructuredGrid<dim>::StructuredGrid
(
	const CoordVector<dim>& lowerBounds,
	const CoordVector<dim>& upperBounds,
	const CoordVector<dim, std::size_t>& resolution
)
{}



template <std::size_t dim>
std::size_t StructuredGrid<dim>::num_vertices() const
{
    double product = 1.0;

    for(size_t i = 0; i < dim-1; i++){
        product *= m_h[i];
    }
}


template <std::size_t dim>
void StructuredGrid<dim>::vertex_coords(CoordVector<dim>& coordsOut, std::size_t ind) const
{
    // TODO: implement
    throw std::runtime_error("not implemented");

}


template <std::size_t dim>
std::size_t StructuredGrid<dim>::index(const CoordVector<dim, std::size_t>& vMultiIndex) const
{
    // Finde den Index einer Vertex gegeben der Koordinaten im Grid
    // the big challenges are finding the relative Position in the grid and then calculating the index

    Vector Multiplkatoren = Vector(dim);        // this will save our lexicographic ordering factors
    double temp_product = num_vertices();       // we will divide resolution from this number

    // define (dim)-many multiplicators for the indexing

    for(size_t i = 0; i < dim-1; i++){
        temp_product /= m_resolution[i];        // Since we want to start counting in the direction of the x-axis
        Multiplkatoren[i] = temp_product;       // for a 24 vertex grind, these factors could look like this: [6,4,1]
    }

    // Since we only have the absolute position of a vertex, we now seek the relative Koordinate of a vertex in the grid
    // Given the lowerBounds and the Schrittweite, which we derive from the resolution, we yield the sought information

    Vector relative_position_in_grid = Vector(dim);
    for(size_t i = 0; i < dim-1; i++){
        relative_position_in_grid[i] = (vMultiIndex[i]- m_lowerBnds[i]) / ((m_upperBnds[i]-m_lowerBnds[i])/(m_resolution[i]-1));
    }

    // with the relative position and the ordering factors at hand, we can calculate the position
    size_t index = 0;
    for(size_t i = 0; i < dim-1; i++){
        index += vMultiIndex[i] * relative_position_in_grid[i];
    }
}


template <std::size_t dim>
CoordVector<dim, std::size_t> StructuredGrid<dim>::multi_index(std::size_t ind) const
{

    // Finde Koordinaten einer Vertex im Grid bei gegebenem Index

    CoordVector result = CoordVector<dim>();                                                                            // SHOULDN'T THERE BE FLOATS IN HERE?

    int index = static_cast<int>(ind);

    // like in the index function, we need the Multiplikatoren:

    Vector Multiplkatoren = Vector(dim);        // this will save our lexicographic ordering factors
    double temp_product = num_vertices();       // we will divide resolution from this number

    for(size_t i = 0; i < dim-1; i++){
        temp_product /= m_resolution[i];        // Since we want to start counting in the direction of the x-axis
        Multiplkatoren[i] = temp_product;       // for a 24 vertex grind, these factors could look like this: [6,4,1]
    }


    for(size_t i = 0; i < dim-1; i++) {         // here we divide by the given multiplicators to derive the coordinates
        result[i] = floor(index / Multiplkatoren[i]); //
    }

    return result<dim, double>;                                                                                         // OBVIOUS PROBLEMS WITH SIZE-T TO FLOAT
}


template <std::size_t dim>
void StructuredGrid<dim>::vertex_neighbors(std::vector<std::size_t>& neighborsOut, std::size_t ind) const
{
                                                                                                                        // Neighbors out hat 3 Einträge?! Sollte doch dim*2 Einträge mit je dim Einträgen haben
    // wir prüfen, ob der Knoten an irgendeinem Rand liegt:

    CoordVector Coordinates = CoordVector<dim>();
    Coordinates = multi_index(ind);                                                                                     // Wir wollen die Koordinaten einer Vertex in einem CoordVector ablegen. Wie geht der Zugriff?

    // prüfe, ob mindestens eine Koordinate am unteren oder oberen Rand liegt
    for(size_t i = 0; i < dim-1; i++) {
        if (Coordinates[i] == m_lowerBnds[i]){

        }
    }


    for(size_t i = 0; i < dim-1; i++) {


    }
}


template <std::size_t dim>
bool StructuredGrid<dim>::is_boundary(std::size_t ind) const
{
    // ermittle Koordinaten von Vertex
    CoordVector Coordinates = CoordVector<dim>();
    Coordinates = multi_index(ind);                                                                                     // Wir wollen die Koordinaten einer Vertex in einem CoordVector ablegen. Wie geht der Zugriff?

    // prüfe, ob mindestens eine Koordinate am unteren oder oberen Rand liegt
    for(size_t i = 0; i < dim-1; i++) {
        if (Coordinates[i] == m_lowerBnds[i] or Coordinates[i] == m_upperBnds[i]){
            return true;
        }
    }
    return false;
}


template <std::size_t dim>
const CoordVector<dim>& StructuredGrid<dim>::lower_bnds() const
{
	return m_lowerBnds;
}


template <std::size_t dim>
const CoordVector<dim>& StructuredGrid<dim>::upper_bnds() const
{
	return m_upperBnds;
}


template <std::size_t dim>
const CoordVector<dim, std::size_t>& StructuredGrid<dim>::mesh_sizes() const
{
	return m_resolution;
}


template <std::size_t dim>
const CoordVector<dim>& StructuredGrid<dim>::element_sizes() const
{
	return m_h;
}



// explicit template declarations
template class StructuredGrid<1>;
template class StructuredGrid<2>;
template class StructuredGrid<3>;
