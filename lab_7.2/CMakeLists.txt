cmake_minimum_required(VERSION 3.5)

project(openmp_7-2 LANGUAGES CXX)

find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

option( DEBUG FALSE )

if ( DEBUG )
    add_definitions( -DDEBUG )
endif()

add_executable(openmp_7-2
    main.cc
    )

target_link_libraries(openmp_7-2 -fopenmp)
