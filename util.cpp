#include "util.h"

template <std::size_t dim>
void sample_function_to_grid_vector
(
	Vector& gvOut,
	const StructuredGrid<dim>& grid,
	double (*fct)(const CoordVector<dim>&)
)
{
	// resize vector
	std::size_t nVrt = grid.num_vertices();
	gvOut.resize(nVrt);

	// sample function
	for (size_t i = 0; i < nVrt; ++i)
	{
		CoordVector<dim> coords;
		grid.vertex_coords(coords, i);
		gvOut[i] = fct(coords);
	}

#ifdef PARALLEL
	gvOut.set_storage_type(PST_CONSISTENT);
#endif
}



// explicit template declarations
template void sample_function_to_grid_vector<1>(Vector&, const StructuredGrid<1>&, double (*)(const CoordVector<1>&));
template void sample_function_to_grid_vector<2>(Vector&, const StructuredGrid<2>&, double (*)(const CoordVector<2>&));
template void sample_function_to_grid_vector<3>(Vector&, const StructuredGrid<3>&, double (*)(const CoordVector<3>&));
