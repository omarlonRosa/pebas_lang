cmake_minimum_required(VERSION 3.15)
project(PebasLang VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configurações para desenvolvimento (warnings, etc.)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g")
endif()

# Configurações específicas para macOS
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# Adicionar subdiretório src
add_subdirectory(src)

# Configurar testes (se usar CTest)
# enable_testing()
# add_subdirectory(tests)

# Mensagem de finalização
message(STATUS "PebasLang configured. Build with 'cmake --build build'")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

