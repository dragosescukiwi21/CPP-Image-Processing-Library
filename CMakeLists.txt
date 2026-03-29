cmake_minimum_required(VERSION 3.10)

# Project name and version
project(CNN_Image_Processing VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directory to the search path for headers
include_directories(include)

# Find all source files in the src/ directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Platform-specific configurations (e.g., standard warnings)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
