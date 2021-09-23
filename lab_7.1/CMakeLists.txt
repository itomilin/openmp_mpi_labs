cmake_minimum_required(VERSION 3.5)

project(openmp_7-1 LANGUAGES C)

find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")

add_executable(openmp_7-1
    main.c
    )

target_link_libraries(openmp_7-1 -fopenmp)
