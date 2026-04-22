cmake_minimum_required(VERSION 3.20)

# Define the project name and language
project(DsaCourse VERSION 1.0 LANGUAGES CXX)

# Enforce a global standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add our subdirectories
add_subdirectory(bl_core)
add_subdirectory(bl_app)

# We will add this in the next lesson
# add_subdirectory(benchmarks)
