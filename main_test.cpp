//
// Created by Ghost on 09.12.2023.
//

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


    Vector a =  Vector(25, 2);
    std::cout << a << std::endl;

    //create structured grid with some values
    const size_t dim = 2;

    CoordVector<dim> lowerBounds(0);
    CoordVector<dim> upperBounds(1);
    //upperBounds[1] = 100.0;

    CoordVector<dim, size_t> resolution(100);
    resolution[0] = 100;
    resolution[1] = 100;

    StructuredGrid<dim> grid(lowerBounds,upperBounds,resolution);


    //run some tests on the structured grid...
    //you can comment or delete these in case
    //they get annoying
    int c = 0;
    for(size_t i = 0; i < resolution[1]; ++i){
        for(size_t j = 0; j < resolution[0]; ++j){
            for(size_t k = 0; k < 1 ; ++k){
                CoordVector<dim, size_t> multi_index(0);
                multi_index[0] = j;
                multi_index[1] = i;

                //std::cout << multi_index << std::endl;
                //std::cout << "Hallo bitte bescheid geben" << std::endl;
                size_t ind = grid.index(multi_index);
                //std::cout << ind << std::endl;

                CoordVector<dim> coords;
                grid.vertex_coords(coords, ind);
                //std::cout << "coordinates\n";
                //std::cout << coords << std::endl;
                if(grid.is_boundary(ind)) ++c;
                //std::cout << "neighbors: ";
                std::vector<std::size_t> neighborsOut;
                grid.vertex_neighbors(neighborsOut, ind);
                for(unsigned long d : neighborsOut){
                    //std::cout << grid.multi_index(d) << "    ";
                }
                //std::cout  << "\n"<< std::endl;
            }
        }
    }
    //number of boundary vertices to inner vertices
    std::cout << "n bnd: " << c << std::endl;
    std::cout << "n inner: " <<  grid.num_vertices() - c << std::endl;

    SparseMatrix S = SparseMatrix();
    Vector rhs = Vector(grid.num_vertices());
    Vector u = Vector(grid.num_vertices());
    for (std::size_t i = 0;  i < u.size(); ++i)
        u[i] =  10 * std::rand() / (double) RAND_MAX;

    IterativeSolver<SparseMatrix> iterative_solver(S);
    iterative_solver.set_convergence_params(50, 1e-15, 1e-8);
    //with set_verbose we should get a print every ~ k iterations
    //with information about current reduction rate etc.
    iterative_solver.set_verbose(true);

    GaussSeidel<SparseMatrix> gs;

    //std::cout << gs << std::endl;

    iterative_solver.set_corrector(&gs);
    auto testi_var = iterative_solver.init(u);

    std::cout << "testi_var: " << testi_var << std::endl;

    //start iteration scheme
    bool success = iterative_solver.solve(u, rhs);

    std::cout << success << std::endl;


    /*
    //create structured grid with some values
    const size_t dim = 2;

    CoordVector<dim> lowerBounds(0);
    CoordVector<dim> upperBounds(1);
    //upperBounds[1] = 100.0;

    CoordVector<dim, size_t> resolution(100);
    resolution[0] = 100;
    resolution[1] = 100;

    StructuredGrid<dim> grid(lowerBounds,upperBounds,resolution);


    //run some tests on the structured grid...
    //you can comment or delete these in case
    //they get annoying
    int c = 0;
    for(size_t i = 0; i < resolution[1]; ++i){
        for(size_t j = 0; j < resolution[0]; ++j){
            for(size_t k = 0; k < 1 ; ++k){
                CoordVector<dim, size_t> multi_index(0);
                multi_index[0] = j;
                multi_index[1] = i;

                std::cout << multi_index << std::endl;
                size_t ind = grid.index(multi_index);
                std::cout << ind << std::endl;

                CoordVector<dim> coords;
                grid.vertex_coords(coords, ind);
                std::cout << "coordinates\n";
                std::cout << coords << std::endl;
                if(grid.is_boundary(ind)) ++c;
                std::cout << "neighbors: ";
                std::vector<std::size_t> neighborsOut;
                grid.vertex_neighbors(neighborsOut, ind);
                for(unsigned long d : neighborsOut){
                    std::cout << grid.multi_index(d) << "    ";
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


    //set dirichlet bnd function and rhs function
    sp_elemDisc->set_dirichlet_boundary(&dirichletBndFct);
    sp_elemDisc->set_rhs(&rhsFct);

    //create VTKExporter and set analytic solution for later comparison
    auto pVTK = new VTKExporter<dim>(grid);
    pVTK->set_function(dirichletBndFct);



    //create algebra objects for S*u = rhs
    SparseMatrix S = SparseMatrix();
    Vector rhs = Vector(grid.num_vertices());
    Vector u = Vector(grid.num_vertices());
    for (std::size_t i = 0;  i < u.size(); ++i)
        u[i] =  10 * std::rand() / (double) RAND_MAX;

    //some test prints
    pVTK->export_vector(u, "u", "u0.vtk");
    pVTK->export_delta(u, "delta0", "delta0.vtk");

    //assemble the discretization of poisson equation:
    sp_elemDisc->assemble(S, rhs, u, grid);

    //create iterative solver and set parameters
    IterativeSolver<SparseMatrix> iterative_solver(S);
    iterative_solver.set_convergence_params(500000, 1e-15, 1e-8);
    //with set_verbose we should get a print every ~ k iterations
    //with information about current reduction rate etc.
    iterative_solver.set_verbose(true);



    //now set gauss seidel as preconditioner of iterative solver:
    GaussSeidel<SparseMatrix> gs;
    iterative_solver.set_corrector(&gs);
    iterative_solver.init(u);


    //activate printing of difference to the analytical solution
    //and set VTKExporter
    iterative_solver.set_printDelta(true);
    iterative_solver.set_vtk(pVTK);

    //start iteration scheme
    bool success = iterative_solver.solve(u, rhs);

    //print final solution
    pVTK->export_vector(u, "u", "poisson2d.vtk");

    //finished! Good luck!
     */


    return 0;
}