message("TPCSim Library")

# if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE Release)
# endif()

# set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -funroll-loops -march=native")
set(CMAKE_CXX_FLAGS "-Werror -Wall -Wpedantic")

# Set the default verbosity of the compilation...
set(CMAKE_VERBOSE_MAKEFILE TRUE)

###################################################

# Configure the dependencies
find_package(ROOT REQUIRED
  COMPONENTS Geom Physics Matrix MathCore Tree RIO)
if(ROOT_FOUND)
  include(${ROOT_USE_FILE})
endif(ROOT_FOUND)

# Define the source and include files that should be used for the io
# part of ERepSim.

set(source_IO
  Reader.cxx Flux.cxx FluxManager.cxx Calculator.cxx Writer.cxx
  )

set(includes_IO
  Reader.h Flux.h FluxManager.h Calculator.h Writer.h
  )

############################driftSim############################

add_library(WeightorIO SHARED ${source_IO} ${includes_IO})

target_include_directories(WeightorIO PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
  "$<INSTALL_INTERFACE:include/WeightorIO>")

install(TARGETS WeightorIO
  EXPORT WeightorIOTargets
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include/WeightorIO)

# Install the header files.
install(FILES ${includes_IO} DESTINATION include/WeightorIO)

# Make sure the current directories are available for the later
# compilation.  This is required to make ROOT_GENERATE_DICTIONARY
# work.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
