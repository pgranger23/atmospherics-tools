# Build all of the applications here.
project(app)

# if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE Release)
# endif()

# set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -funroll-loops -march=native")
set(CMAKE_CXX_FLAGS "-Werror -Wall -Wpedantic")

# Set the default verbosity of the compilation...
set(CMAKE_VERBOSE_MAKEFILE TRUE)

find_package(ROOT REQUIRED
  COMPONENTS Geom Physics Matrix MathCore Tree RIO)
if(ROOT_FOUND)
  include(${ROOT_USE_FILE})
endif(ROOT_FOUND)

add_executable(weightor weightor.cxx)
target_link_libraries(weightor
  LINK_PUBLIC WeightorIO ROOT::EG ${ROOT_LIBRARIES} OscLib argparse)

add_executable(split_channels split_channels.cxx)
target_link_libraries(split_channels
  LINK_PUBLIC WeightorIO ROOT::EG ${ROOT_LIBRARIES})

# add_executable(fit_curvature fit_curvature.cxx)
# target_link_libraries(fit_curvature
#   LINK_PUBLIC driftSim ${ROOT_LIBRARIES} matplotlib_cpp ROOT::EG ROOT::Minuit)

# add_executable(asimov asimov.cxx)
# target_link_libraries(asimov
#   LINK_PUBLIC driftSim ${ROOT_LIBRARIES} matplotlib_cpp ROOT::EG ROOT::Minuit)

# add_executable(compare_3G compare_3G.cxx)
# target_link_libraries(compare_3G
#   LINK_PUBLIC driftSim ${ROOT_LIBRARIES} ROOT::EG matplotlib_cpp)

# find_package(OpenMP)
# if(OpenMP_CXX_FOUND)
#     target_link_libraries(build_displacement_map PUBLIC OpenMP::OpenMP_CXX)
# endif()

install(TARGETS weightor split_channels RUNTIME DESTINATION bin)

