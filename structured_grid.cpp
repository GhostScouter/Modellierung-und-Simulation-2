

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

    Vector Multiplikatoren = Vector(dim);        // this will save our lexicographic ordering factors
    double temp_product = num_vertices();        // we will divide resolution from this number

    // define (dim)-many multiplicators for the indexing

    for(size_t i = 0; i < dim-1; i++){
        temp_product /= m_resolution[i];         // Since we want to start counting in the direction of the x-axis
        Multiplikatoren[i] = temp_product;       // for a 24 vertex grind, these factors could look like this: [6,4,1]
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

    // Finde Koordinaten eines Vertex im Grid bei gegebenem Index

    CoordVector result = CoordVector<dim, std::size_t>();

    int index = static_cast<int>(ind);

    // like in the index function, we need the Multiplikatoren:

    Vector Multiplkatoren = Vector(dim);        // this will save our lexicographic ordering factors
    double temp_product = num_vertices();       // we will divide resolution from this number

    for(size_t i = 0; i < dim-1; i++){
        temp_product /= m_resolution[i];        // Since we want to start counting in the direction of the x-axis
        Multiplkatoren[i] = temp_product;       // for a 24 vertex grind, these factors could look like this: [6,2,1]
    }


    for(size_t i = 0; i < dim-1; i++) {         // here we divide by the given multiplicators to derive the coordinates
        result[i] = floor(index / Multiplkatoren[i]);       // Ergebnisse sind floored und daher size_t
    }

    return result;
}


template <std::size_t dim>
void StructuredGrid<dim>::vertex_neighbors(std::vector<std::size_t>& neighborsOut, std::size_t ind) const
{
    for(size_t i = 0; i < dim; i ++){      // first, we generate all possible neighbors

        neighborsOut[i] = ind + ((m_upperBnds[i]-m_lowerBnds[i])/(m_resolution[i]-1));      // the calculation returns the Schrittweite of our grid
        neighborsOut[2*i] = ind - ((m_upperBnds[i]-m_lowerBnds[i])/(m_resolution[i]-1));    // essentially, we do one step in both directions

    }

    // wir prüfen nun die Exitenz der Nachbarn
    // zuerst bilden wir den "Faktoren" Vektor, der zur prüfung hilfreich ist

    Vector Multiplikatoren = Vector(dim);               // this will save our lexicographic ordering factors
    Vector factorisation_vertex = Vector(dim);          // save factorisation of vertex
    Vector factorisation_neighbor = Vector(dim);        // save factorisation of neighbor
    double temp_product = num_vertices();               // we will divide resolution from this number

    for(size_t i = 0; i < dim-1; i++){
        temp_product /= m_resolution[i];                // Since we want to start counting in the direction of the x-axis
        Multiplikatoren[i] = temp_product;              // for a 24 vertex grind, these factors could look like this: [6,2,1]
    }

    for(int j = 0; j < dim; j++) {                   // find factorisation of vertex to compare to neighbor
        factorisation_vertex[j] = ind / Multiplikatoren[j];
    }


    for(size_t i = 0; i < 2*dim; i++){
        CoordVector KoordinateEinesNachbarn = multi_index(i);   // Koordinatendarstellung des Nachbarn
        int error = 0;

        if(neighborsOut[i] < 0 || ind > num_vertices()-1){          // the index cant be lower than zero or higher than X
            neighborsOut.erase(i);
        }

        // This is trickier: We also must make sure only real neighbors are neighbors

        for(int j = 0; j < dim; j++){                               // find factorisation of neighbor and compare to vertex
            factorisation_neighbor [j] = ind / Multiplikatoren[j];

            if(factorisation_neighbor[j] != factorisation_vertex[j]) {  // if 2 Factors are different, the vertices are not neigbors!
                neighborsOut.erase[i];
            }
        }
    }
}


template <std::size_t dim>
bool StructuredGrid<dim>::is_boundary(std::size_t ind) const
{
    // ermittle Koordinaten von Vertex
    CoordVector Coordinates = multi_index(ind);      // Speichert Multi-Koordinaten der Vertex

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
