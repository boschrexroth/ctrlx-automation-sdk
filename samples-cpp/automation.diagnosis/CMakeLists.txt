#
# Set CMAKE version
#
cmake_minimum_required( VERSION 3.9 )

#
# Project information
#
set(VERSION 1.0.0)
project( automation_diagnosis_project VERSION ${VERSION} )

#
# User dependency directories
#
set (USER_DEPENDENCY_DIR ${CMAKE_CURRENT_LIST_DIR}/../../)
set (DIAGNOSIS_DEPENDENCY_DIR ${USER_DEPENDENCY_DIR}/lib/common.log.diagnosis/)

#
# Option to Build the Snap
#
option (BUILD_SNAP "Build Snap Package" OFF)

MESSAGE( STATUS "Selected BuildKit: ${CMAKE_BUILD_KIT}")
MESSAGE( STATUS "######################################")
MESSAGE( STATUS "Option: BUILD_SNAP ${BUILD_SNAP}")
MESSAGE( STATUS "######################################")

#
# Compiler setting
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

#
# set libraries dirs and includes
#
if( UNIX )
  IF( ${CMAKE_BUILD_KIT} MATCHES "aarch64")
    MESSAGE(STATUS "Set Platformname to: ubuntu20-gcc-aarch64")
    set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
    set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
    SET(PLATFORM_NAME "ubuntu20-gcc-aarch64")
    SET(SNAP_ARCH "arm64")
  ELSE()
    MESSAGE(STATUS "Set Platformname to: ubuntu20-gcc-x64")
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    SET(PLATFORM_NAME "ubuntu20-gcc-x64")
    SET(SNAP_ARCH "amd64")
    ENDIF()
endif()

link_directories(
  ${TRACE_DEPENDENCY_DIR}/${PLATFORM_NAME}/release
  ${USER_DEPENDENCY_DIR}/bin/common.log.diagnosis/${PLATFORM_NAME}/release
)

#
# Set CMAKE-Celix Variables 
#
set(BUNDLEX_PUBLIC_INSTALL_DIR ${CMAKE_SOURCE_DIR}/generated/automation-diagnosis)

# includes
set(CELIX_FOUND false)
set(CELIX_LAUNCHER celix)
set(CELIX_INCLUDE_DIR
  ${USER_DEPENDENCY_DIR}/include
  ${USER_DEPENDENCY_DIR}/include/oss.celix
  ${USER_DEPENDENCY_DIR}/include/oss.celix/utils
  ${USER_DEPENDENCY_DIR}/include/oss.celix/dependency_manager_cxx
  ${USER_DEPENDENCY_DIR}/include/oss.celix/dependency_manager
)
set(CELIX_ETCD_INCLUDE_DIR ${CELIX_INCLUDE_DIR})

# libraries 
set(CELIX_LIBRARIES ${USER_DEPENDENCY_DIR}/bin/oss.celix/${PLATFORM_NAME}/release)
set(CELIX_FRAMEWORK_LIBRARY ${CELIX_LIBRARIES}/libcelix_framework.so)
set(CELIX_UTILS_LIBRARY ${CELIX_LIBRARIES}/libcelix_utils.so)
set(CELIX_DFI_LIBRARY ${CELIX_LIBRARIES}/libcelix_dfi.so)
set(CELIX_DM_LIB ${CELIX_LIBRARIES}/libdependency_manager_so.so)
set(CELIX_DM_STATIC_LIB ${CELIX_LIBRARIES}/libdependency_manager_static.a)
set(CELIX_DM_STATIC_CXX_LIB ${CELIX_LIBRARIES}/libdependency_manager_cxx_static.a)

# bundle
set(CELIX_CMAKECELIX_FILE ${USER_DEPENDENCY_DIR}/cmake/oss.celix/CMakeCelix.cmake)

set(CMAKE_MODULE_PATH  "${USER_DEPENDENCY_DIR}/cmake/oss.celix" ${CMAKE_MODULE_PATH})
find_package(CELIX REQUIRED)

#
# Loop over all projects within subfolders
#
file ( GLOB_RECURSE PROJECTS ${CMAKE_SOURCE_DIR}/*/CMakeLists.txt )
foreach ( PRJ ${PROJECTS} )
  message( STATUS "" )
  message( STATUS "============================================" )
  message( STATUS "Creating project for ${PRJ}" )
  message( STATUS "============================================" )

  get_filename_component( PRJ_PATH ${PRJ} DIRECTORY )
  message( STATUS " adding project " ${PRJ_PATH} )
  add_subdirectory( ${PRJ_PATH} )
endforeach()

#
# Build dummy executable for make install
#
add_executable(dummy
  ${CMAKE_SOURCE_DIR}/private/src/dummy.cpp
)


add_custom_command( TARGET dummy
  POST_BUILD
  COMMAND make install
)

if (BUILD_SNAP)
  add_custom_command( TARGET dummy
    POST_BUILD
    COMMAND snapcraft --enable-experimental-target-arch --target-arch=${SNAP_ARCH} --destructive-mode
    COMMAND snapcraft clean --destructive-mode
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
endif(BUILD_SNAP)
