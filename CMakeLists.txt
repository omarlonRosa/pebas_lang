cmake_minimum_required(VERSION 3.15)
project(PebasLang VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PEBAS_BUILD_TESTS "Build tests" ON)
option(PEBAS_BUILD_EXAMPLES "Build examples" ON)

include_derectories(include)

add_subdirectory(src)

if(PEBAS_BUILD_TESTS)
enable_testing()
add_subdirectory(tests)
endif()

if(PEBAS_BUILD_EXAMPLES)
add_subdirectory(examples)
endif()

