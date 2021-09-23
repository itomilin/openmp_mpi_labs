cmake_minimum_required(VERSION 3.5)

project(openmpi10_11 LANGUAGES CXX)

find_package(MPI REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(openmpi10_11
    main.cc
    )

target_link_libraries(openmpi10_11 ${MPI_CXX_LIBRARIES} -fopenmp)
