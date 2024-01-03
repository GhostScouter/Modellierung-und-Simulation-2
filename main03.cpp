
#include "poisson_disc.h"
#include "vtk_exporter.h"
#include "vector.h"
#include "matrix.h"
#include "sparse_matrix.h"
#include "iterative_solver.h"
#include "jacobi.h"
#include "gauss_seidel.h"
#include "lu_solver.h"
#include "structured_grid.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <memory>


void printVectorOnGrid(Vector& u, size_t nElemsPerDim) {

    for (size_t i = 0; i < u.size(); ++i) {

        if (i % (nElemsPerDim) == 0) {
            std::cout << std::endl;
        }
        std::cout << std::setw(6) << std::setfill(' ') << std::setprecision(2) << std::fixed << u[i] << "\t";

    }
}
template <size_t dim>
bool dirichletBndFct(double& value, const CoordVector<dim>& coords){
    value = 1 - 0.5 * coords[0] * coords[0] - 0.5 * coords[1] * coords[1];
    return true;
}
template <size_t dim>
double rhsFct(const CoordVector<dim>& coords){
    return 2.0;
}


int main(int argc, char** argv){


    //create structured grid with some values
    const size_t dim = 2;

    CoordVector<dim> lowerBounds(0);
    CoordVector<dim> upperBounds(1);
    //upperBounds[1] = 100.0;

    CoordVector<dim, size_t> resolution(100);
    resolution[0] = 3;
    resolution[1] = 3;

    //TEST DIM DREI
    //resolution[0] = 4;
    //resolution[1] = 2;
    //resolution[1] = 2;

    StructuredGrid<dim> grid(lowerBounds,upperBounds,resolution);


    //run some tests on the structured grid...
    //you can comment or delete these in case
    //they get annoying
    int c = 0;
    for(size_t i = 0; i < resolution[1]; ++i){
        for(size_t j = 0; j < resolution[0]; ++j){
            for(size_t k = 0; k < 1 ; ++k){

    //TEST DIM DREI
    //for(size_t i = 0; i < resolution[2]; ++i){
        //for(size_t j = 0; j < resolution[1]; ++j){
            //for(size_t k = 0; k < resolution[0] ; ++k){

                CoordVector<dim, size_t> multi_index(0);
                multi_index[0] = j;
                multi_index[1] = i;

                //TEST DIM DREI
                //multi_index[0] = k;
                //multi_index[1] = j;
                //multi_index[2] = i;

                std::cout << multi_index << std::endl;
                size_t ind = grid.index(multi_index);
                std::cout << ind << std::endl;
                std::cout << grid.multi_index(ind) <<"\n\n" <<  std::endl;


                CoordVector<dim> coords;
                grid.vertex_coords(coords, ind);
                std::cout << "coordinates\n";
                std::cout << coords << std::endl;
                if(grid.is_boundary(ind)) ++c;
                std::cout << "neighbors: ";
                std::vector<std::size_t> neighborsOut;
                grid.vertex_neighbors(neighborsOut, ind);
                for(unsigned long d : neighborsOut){
                    std::cout << d << "    ";
                }
                std::cout  << "\n"<< std::endl;
            }
        }
    }
    //number of boundary vertices to inner vertices
    std::cout << "n bnd: " << c << std::endl;
    std::cout << "n inner: " <<  grid.num_vertices() - c << std::endl;

    //create Poisson discretization (as a smart pointer)
    typedef PoissonDisc<dim, SparseMatrix> TElemDisc;
    typedef std::unique_ptr<PoissonDisc<dim, SparseMatrix>> T_spElemDisc;
    T_spElemDisc sp_elemDisc = std::make_unique<TElemDisc>();

    //create algebra objects for S*u = rhs
    SparseMatrix S = SparseMatrix();
    Vector rhs = Vector(grid.num_vertices());
    Vector u = Vector(grid.num_vertices());
    for (std::size_t i = 0;  i < u.size(); ++i)
        u[i] =  10 * std::rand() / (double) RAND_MAX;


    //set dirichlet bnd function and rhs function
    sp_elemDisc->set_dirichlet_boundary(&dirichletBndFct);
    sp_elemDisc->set_rhs(&rhsFct);

    //assemble the discretization of poisson equation:
    sp_elemDisc->assemble(S, rhs, u, grid);
    //create iterative solver and set parameters
    IterativeSolver<SparseMatrix> iterative_solver(S);
    iterative_solver.set_convergence_params(500000, 1e-15, 1e-8);
    //with set_verbose we should get a print every ~ k iterations
    //with information about current reduction rate etc.
    iterative_solver.set_verbose(true);

    //now set gauss seidel as preconditioner of iterative solver:
    Jacobi<SparseMatrix> jac;
    iterative_solver.set_corrector(&jac);
    iterative_solver.init(u);

    //create VTKExporter and set analytic solution for later comparison
    auto pVTK = new VTKExporter<dim>(grid);
    // pVTK->set_function(dirichletBndFct);




    //some test prints
    pVTK->export_vector(u, "u", "u0.vtk");
    // pVTK->export_delta(u, "delta0", "delta0.vtk");


    std::cout << "starting--" << std::endl;






    //start iteration scheme
    bool success = iterative_solver.solve(u, rhs);
    if(success){std::cout << "converged..." << std::endl;}
    //print final solution
    pVTK->export_vector(u, "u", "poisson2d.vtk");

    //finished! Good luck!


    return 0;
}