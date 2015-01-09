# - Config file for the libnabo package
# It defines the following variables
#  libnabo_INCLUDE_DIRS - include directories for libnabo
#  libnabo_LIBRARIES    - libraries to link against
 
# Compute paths
get_filename_component(libnabo_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(libnabo_INCLUDE_DIRS "/home/louis/DOT_Indev/3rd_party/include")
 
# Our library dependencies (contains definitions for IMPORTED targets)
if(NOT TARGET nabo AND NOT libnabo_BINARY_DIR)
  include("${libnabo_CMAKE_DIR}/libnaboTargets.cmake")
endif()
 
# These are IMPORTED targets created by libnaboTargets.cmake
if (CMAKE_COMPILER_IS_GNUCC)
  set(libnabo_LIBRARIES /home/louis/DOT_Indev/3rd_party/libnabo-master/libnabo.a gomp)
else(CMAKE_COMPILER_IS_GNUCC)
  set(libnabo_LIBRARIES /home/louis/DOT_Indev/3rd_party/libnabo-master/libnabo.a)
endif(CMAKE_COMPILER_IS_GNUCC)

# This causes catkin_simple to link against these libraries
set(libnabo_FOUND_CATKIN_PROJECT true)
