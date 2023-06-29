cmake_minimum_required(VERSION 3.0)

# Set the project name and version.  The project name will be used to
# import this into other CMAKE files.
project(Weightor VERSION 1.0.0)

message("Weightor -- ${VERSION}")

# Define the options that can be set in the cache, or on the cmake
# command line.
set(CMAKE_BUILD_TYPE Debug)


# Make sure that ROOT is available.  ROOT is absolutely required.
find_package(ROOT REQUIRED
  COMPONENTS Geom Physics Matrix MathCore Tree RIO EG)
if(ROOT_FOUND)
  include(${ROOT_USE_FILE})
endif(ROOT_FOUND)

add_subdirectory(src)
add_subdirectory(app)