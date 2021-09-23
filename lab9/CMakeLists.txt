cmake_minimum_required(VERSION 3.5)

project(openmpi9 LANGUAGES CXX)

find_package(MPI REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

option( DEBUG FALSE )
if ( DEBUG )
    add_definitions( -DDEBUG )
endif()

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(openmpi9
    main.cc
    )

target_link_libraries(openmpi9 ${MPI_CXX_LIBRARIES} -fopenmp)
