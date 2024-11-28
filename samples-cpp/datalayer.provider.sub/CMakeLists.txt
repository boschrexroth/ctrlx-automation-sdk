#
# Set CMAKE version
#
cmake_minimum_required( VERSION 3.9 )

#
# Project information
#
project( datalayer_provider_sub VERSION 3.0.0 )

#
# Set TARGET_PROJECT_NAME for each executable (same as snap "app name" =ExeName)
#
set(TARGET_PROJECT_NAME datalayerProviderSub)

#
#
# User dependency directories
#
set (SDK_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../)

#
# Option to Build the Snap
#
option (BUILD_SNAP "Build Snap Package" OFF)

MESSAGE( STATUS "Selected BuildKit: ${BUILD_KIT}, ${TARGET_PROJECT_NAME}")
MESSAGE( STATUS "######################################")
MESSAGE( STATUS "Option: BUILD_SNAP ${BUILD_SNAP}")
MESSAGE( STATUS "######################################")

#
# Compiler setting
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")


#
# set libraries dirs and includes
#
IF( ${BUILD_KIT} MATCHES "aarch64")
  SET(PLATFORM_NAME "ubuntu22-gcc-aarch64")
  SET(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
ELSE()
  SET(PLATFORM_NAME "ubuntu22-gcc-x64")
  SET(CMAKE_CXX_COMPILER g++)
ENDIF()


#
# set private include directories
#

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include 
  )

add_subdirectory(src)


if (BUILD_SNAP)
  #
  # Set destination paths for installation
  #
  install( TARGETS provider client
    RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/build
    LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/build
    ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/build
    COMPONENT ${BUNDLE_PROJECT_NAME}
  )
endif(BUILD_SNAP)

