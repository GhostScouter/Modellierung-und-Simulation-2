

#include "structured_grid.h"

#include <cmath>

#include <stdexcept>  // for std::runtime_error
#include <iostream>


template <std::size_t dim>
StructuredGrid<dim>::StructuredGrid
(
	const CoordVector<dim>& lowerBounds,
	const CoordVector<dim>& upperBounds,
	const CoordVector<dim, std::size_t>& resolution
)
{
    m_lowerBnds = lowerBounds;
    m_upperBnds = upperBounds;
    m_resolution = resolution;
}


template <std::size_t dim>
std::size_t StructuredGrid<dim>::num_vertices() const
{
    size_t product = 1;

    for(size_t i = 0; i < dim; i++){
        product *= m_resolution[i];
    }
    return product;
}


template <std::size_t dim>
void StructuredGrid<dim>::vertex_coords(CoordVector<dim>& coordsOut, std::size_t ind) const
{
    // Diese Funktion soll von einem Index die "wahren Koordinaten" ermitteln
    // nehme multikoordinaten
    // berechne mit (Start + Schritte * Schrittweite) die vertex_coords
    // schreibe in coordsOut

    CoordVector<dim, size_t> temp_coords;         // soll die multiindex Koordinaten halten
    temp_coords = multi_index(ind);

    for(size_t i = 0; i < dim; i++){
        coordsOut[i] = m_lowerBnds[i] + temp_coords[i]*((m_upperBnds[i]-m_lowerBnds[i])/m_resolution[i]);
        // Startpunkt des Grids plus Schritte * Schrittweite
    }
}


template <std::size_t dim>
std::size_t StructuredGrid<dim>::index(const CoordVector<dim, std::size_t>& vMultiIndex) const
{
    // Diese Funktion soll, gegeben eines Multiindexes, den Index finden

    Vector Multiplikatoren = Vector(dim);              // this will save our lexicographic ordering factors
    size_t number_of_vertices = num_vertices();        // we will divide resolution from this number

    // define (dim)-many multiplicators for the indexing

    for(size_t i = 0; i < dim; i++){
        //std::cout << m_resolution[i] << std::endl;
        number_of_vertices /= m_resolution[i];         // Since we want to count in the direction of the x-axis first
        Multiplikatoren[i] = number_of_vertices;       // for a 24 vertex grid, these factors could look like this: [6,2,1]
    }

    // with multiindex and multiplicators at hand, we can calculate the position easily

    size_t index = 0;                                   // start at 0, if the vertex is at position (0,0,0)
    for(size_t i = 0; i < dim; i++){                    // for each dimention, we increade the index by the multiindex position * multiplier
        index += vMultiIndex[i] * Multiplikatoren[i];   // for every step into x-direction, we add the x-multiplier (here: 6)
    }

    return index;
}


template <std::size_t dim>
CoordVector<dim, std::size_t> StructuredGrid<dim>::multi_index(std::size_t ind) const
{
    // Finde Koordinaten eines Vertex im Grid bei gegebenem Index
    // generiere Variablen
    CoordVector<dim, std::size_t> results;     // erstelle den results Coordvector
    int index = static_cast<int>(ind);

    // like in the index function, we need the Multiplikatoren:

    Vector Multiplkatoren = Vector(dim);            // this will save our lexicographic ordering factors
    size_t temp_product = num_vertices();           // we will divide resolution from this number

    for(size_t i = 0; i < dim; i++){
        temp_product /= m_resolution[i];            // Since we want to start counting in the direction of the x-axis
        Multiplkatoren[i] = temp_product;           // for a 24 vertex grind, these factors could look like this: [6,2,1]
    }

    for(size_t i = 0; i < dim; i++) {               // divide by the given multiplicators to derive coordinates
        results[i] = floor(index / Multiplkatoren[i]);      // Ergebnisse sind floored und daher size_t
        index -= results[i]*Multiplkatoren[i];
    }

    return results;
}


template <std::size_t dim>
void StructuredGrid<dim>::vertex_neighbors(std::vector<std::size_t>& neighborsOut, std::size_t ind) const
{
    // build a neighbour vector from an index
    // transform to multiindex -> add 1 and substract one from each position
    // if > upper_bound oder < lowerboud => error

    CoordVector<dim, std::size_t> temp_coords;          // soll die multiindex Koordinaten halten



    for(size_t i = 0; i < dim; i++){
        temp_coords = multi_index(ind);                                     // halte multiindex des fraglichen Knotens
        for(size_t k = 0; k < dim; ++k){
            temp_coords[k] = multi_index(ind)[dim - k - 1];
        }
        if(temp_coords[i] == 0){               //  if the multiindex is at the lower border, only add a neighbour + 1
            temp_coords[i] += 1;
            neighborsOut.push_back(index(temp_coords));
        }
        else if(temp_coords[i] == m_resolution[i]-1){          //  if the multiindex is at the upper border, only add a neighbour - 1
            temp_coords[i] -= 1;
            neighborsOut.push_back(index(temp_coords));
        }

        else{                                               // we add both neighbors
            temp_coords[i] -= 1;
            neighborsOut.push_back(index(temp_coords));
            temp_coords[i] += 2;
            neighborsOut.push_back(index(temp_coords));   
        }
    }
}


template <std::size_t dim>
bool StructuredGrid<dim>::is_boundary(std::size_t ind) const
{
    // ermittle Koordinaten von Vertex
    CoordVector<dim, std::size_t> temp_coords;      // Speichert Multi-Koordinaten der Vertex
    temp_coords = multi_index(ind);
    // prüfe, ob mindestens eine Koordinate am unteren oder oberen Rand liegt
    for(size_t i = 0; i < dim; i++) {
        if (temp_coords[i] == m_lowerBnds[i] or temp_coords[i] == m_upperBnds[i]){
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
