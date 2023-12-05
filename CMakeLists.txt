cmake_minimum_required (VERSION 3.2)
project (ModSim2)

## options
option(DEBUG "compile in debug mode" ON)
message(STATUS "DEBUG     ${DEBUG}")
option(PARALLEL "compile for parallel usage" OFF)
message(STATUS "PARALLEL     ${PARALLEL}")

## directories
get_filename_component(ROOT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/lib)

# clang does not like -pthread in the linking stage
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(additionalFlags "${additional_flags} -Qunused-arguments")
endif()
set(exclude_warnings "-Wno-unused-parameter")
set(warnings "-Wall -Wextra")
set(cxx_flags "${warnings} ${exclude_warnings} ${additionalFlags} -pedantic -std=c++17")

if (DEBUG)
	set(cxx_flags "${cxx_flags} -g -D__DEBUG__ -O0")
else (DEBUG)
	set(cxx_flags "${cxx_flags} -DNDEBUG -O3")
endif (DEBUG)

set(CMAKE_CXX_FLAGS "" CACHE STRING "clear flags" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxx_flags}" CACHE STRING "overriden flags!" FORCE)


set(linkLibraries)


## ADD ANY SOURCE FILES FOR LIBRARY HERE ##
set(SOURCES
		vector.cpp
		matrix.cpp
		sparse_matrix.cpp
		#structured_grid.cpp
		poisson_disc.cpp
		iterative_solver.cpp
		lu_solver.cpp
		jacobi.cpp
		#gauss_seidel.cpp
		util.cpp
		vtk_exporter.cpp
        main01.cpp
		#main03.cpp
        
)

add_library(modsim2 ${SOURCES} ${linkLibraries})


## targets
add_executable(executable03 main01.cpp)
target_link_libraries(executable03 modsim2 ${linkLibraries})

