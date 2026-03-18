cmake_minimum_required(VERSION 3.22.1)
project(CACHE_PROJECT)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER /usr/bin/g++)


# Add compiler optimizations, warnings, and other options
add_compile_options(-O3 -march=native -mavx2 -fopenmp -Wall -Wextra -Wpedantic)
find_package(OpenMP REQUIRED)
find_package(BLAS REQUIRED)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Library target
add_library(cache_lib
    src/cache.cpp
    src/memory_system.cpp
    src/main.cpp
)

include_directories($ENV{HOME}/.local/include)
link_directories($ENV{HOME}/.local/lib)

target_include_directories(cache_lib PUBLIC include)
# target_link_libraries(cache_lib PUBLIC OpenMP::OpenMP_CXX)
# target_link_libraries(cache_lib PUBLIC BLAS::BLAS)

add_executable(cache_project src/main.cpp)
target_link_libraries(cache_project PRIVATE cache_lib)