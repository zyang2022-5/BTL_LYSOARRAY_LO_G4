cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(main)

find_package(MPI REQUIRED)
add_definitions(-DOMPI_SKIP_MPICXX)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

include_directories(SYSTEM ${MPI_INCLUDE_PATH})


file(GLOB headers ${PROJECT_SOURCE_DIR}/ParallelNSGAII_G4/src_NSGAII/*.h ${PROJECT_SOURCE_DIR}/../src_G4/*.hh)

file(GLOB sources ${PROJECT_SOURCE_DIR}/ParallelNSGAII_G4/src_NSGAII/*.c ${PROJECT_SOURCE_DIR}/ParallelNSGAII_G4/src_NSGAII/*.cpp ${PROJECT_SOURCE_DIR}/../src_G4/*.cc)


file(GLOB MACRO_FILES
    "../*.mac"
)
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

file(GLOB DATA_FILES
    "../data/*.dat"
)

file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

print_all_variables()

add_executable(NSGAII_MSI parallelnsga2r.cpp ${sources} ${headers})
target_link_libraries(NSGAII_MSI ${MPI_C_LIBRARIES} ${CMAKE_DL_LIBS} ${Geant4_LIBRARIES})

add_custom_target(Simulation_Guillermo DEPENDS sim)
