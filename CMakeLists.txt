cmake_minimum_required(VERSION 3.1.0...4.0.0)

project(Raytracer LANGUAGES CXX)

# Set to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
set(SOURCES
    src/main.cc
    src/vec3.h
    src/color.h
)

# Include src directory for header files
include_directories(src)

# Compiler-specific flags for better error checking
message(STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("/W4")      # Enable level-4 warnings
    add_compile_options("/wd 4100") # Disable unreferenced formal parameter warning
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra)      # Enable common warnings
    add_compile_options(-Wunused-variable)  # Warn about unused variables
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra)      # Enable common warnings
    add_compile_options(-Wunused-variable)  # Warn about unused variables
endif()

# Create the executable
add_executable(raytracer ${SOURCES})