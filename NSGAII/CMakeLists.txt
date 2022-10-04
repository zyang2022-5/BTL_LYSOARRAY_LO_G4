cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(main)

find_package(MPI REQUIRED)
add_definitions(-DOMPI_SKIP_MPICXX)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

file(GLOB sources ${PROJECT_SOURCE_DIR}/ParallelNSGAII_G4/src_NSGAII/*.c)

file(GLOB headers ${PROJECT_SOURCE_DIR}/ParallelNSGAII_G4/src_NSGAII/*.h)

file(GLOB headers ${PROJECT_SOURCE_DIR}/ParallelNSGAII_G4/src_NSGAII/*.cpp)

add_executable(NSGAII_MSI ParallelNSGAII_G4/parallelnsga2r.cpp ${sources} ${headers})
target_link_libraries(NSGAII_MSI ${MPI_C_LIBRARIES} ${CMAKE_DL_LIBS})

add_custom_target(Simulation_Guillermo DEPENDS sim)
