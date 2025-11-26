#
# Base CMake file
#
cmake_minimum_required(VERSION 3.20)

#
# Option to Build the Snap
#
option (BUILD_SNAP "Build Snap Package" OFF)

MESSAGE( STATUS "Selected BuildKit: ${BUILD_KIT}")
MESSAGE( STATUS "######################################")
MESSAGE( STATUS "Option: BUILD_SNAP ${BUILD_SNAP}")
MESSAGE( STATUS "######################################")

#
# Compiler setting
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

#
# set libraries dirs and includes
#
IF( ${BUILD_KIT} MATCHES "aarch64")
  SET(PLATFORM_NAME "ubuntu24-gcc-aarch64")
  SET(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
  SET(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
ELSE()
  SET(PLATFORM_NAME "ubuntu24-gcc-x64")
  SET(CMAKE_C_COMPILER gcc)
  SET(CMAKE_CXX_COMPILER g++)
ENDIF()

#
# User dependency directories
#
set (SDK_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../..)


#
# Project information
#
project(scheduler_remote VERSION 3.0.0)

#
# Add further CMakeLists.txt file(s)
#
add_subdirectory(src)


