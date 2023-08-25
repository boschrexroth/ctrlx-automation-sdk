#
# Set CMAKE version
#
cmake_minimum_required( VERSION 3.9 )

#
# Project information
# Set project, VERSION should be same as snap version
#
set(VERSION 2.3.0)
project( register_node_project VERSION ${VERSION} )

#
# Set TARGET_PROJECT_NAME for each executable (same as snap "app name" =ExeName)
#
set(TARGET_PROJECT_NAME registerNode)

#
# User dependency directories
#
set (USER_DEPENDENCY_DIR ${CMAKE_CURRENT_LIST_DIR}/../../)

#
# Metadata Database
#
include(${USER_DEPENDENCY_DIR}/cmake/BuildMDDB.cmake)

set(MDDB_FILES  ${CMAKE_CURRENT_LIST_DIR}/metadata.csv)
build_mddb(registernodemddb "${MDDB_FILES}" ${CMAKE_CURRENT_LIST_DIR}/compiled "")
  
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
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMY_DEBUG")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMY_DEBUG")

#
# set libraries dirs and includes
#
IF( ${CMAKE_BUILD_KIT} MATCHES "aarch64")
  SET(PLATFORM_NAME "ubuntu22-gcc-aarch64")
  SET(SNAP_ARCH "arm64")
  SET(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
  SET(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
ELSE()
  SET(PLATFORM_NAME "ubuntu22-gcc-x64")
  SET(SNAP_ARCH "amd64")
  SET(CMAKE_C_COMPILER gcc)
  SET(CMAKE_CXX_COMPILER g++)
ENDIF()

#
# set source files for each executable
#
set( SOURCE_FILES_REGISTERNODE
  ./main.cpp
)

#
# set private include directories
#
SET ( PRIVATE_INCLUDE_DIRS
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${USER_DEPENDENCY_DIR}/include
  ${USER_DEPENDENCY_DIR}/include/comm.datalayer
  )

#
# Flatbuffers
#
execute_process(COMMAND uname -a OUTPUT_VARIABLE HOST_SYSTEM_ARCH)
IF( ${HOST_SYSTEM_ARCH} MATCHES "aarch64")
  set (FLATBUFFERS_FLATC_EXECUTABLE ${USER_DEPENDENCY_DIR}/bin/oss.flatbuffers/ubuntu22-gcc-aarch64/release/flatc)
ELSE()
  set (FLATBUFFERS_FLATC_EXECUTABLE ${USER_DEPENDENCY_DIR}/bin/oss.flatbuffers/ubuntu22-gcc-x64/release/flatc)
ENDIF()
set (FLATBUFFERS_INCLUDE_DIR ${USER_DEPENDENCY_DIR}/include/oss.flatbuffers)
include (${USER_DEPENDENCY_DIR}/cmake/oss.flatbuffers/FindFlatBuffers.cmake)
set( FLATBUFFERS_FLATC_SCHEMA_EXTRA_ARGS "--gen-object-api")
set( FLATBUFFERS_FLATC_SCHEMA_EXTRA_ARGS "--no-warnings")
set( FLATBUFFERS_FLATC_SCHEMA_EXTRA_ARGS "--gen-compare")

build_flatbuffers("sampleSchema.fbs" "" "registernodeflatbuffers" "${CMAKE_CURRENT_LIST_DIR}" "${CMAKE_CURRENT_LIST_DIR}" "${CMAKE_CURRENT_LIST_DIR}/compiled" "")

#
# Set link directories
#
MESSAGE( STATUS "Libraries directory: ${LIBRARY_DEP_DIR}")
link_directories(
  ${LIBRARY_DIR} 
  ${LIBRARY_DEP_DIR}
  )

#
# Define Executables to add from project files and their depending source files
#
add_executable( ${TARGET_PROJECT_NAME}
	${SOURCE_FILES_REGISTERNODE}
  )

add_dependencies(${TARGET_PROJECT_NAME} 
  registernodeflatbuffers 
  registernodemddb
  )

#
# Set target include directories
#
target_include_directories ( ${TARGET_PROJECT_NAME}
  PUBLIC ${PUBLIC_INCLUDE_DIRS}
  PUBLIC ${LIBRARY_INCLUDES}
	PRIVATE ${PRIVATE_INCLUDE_DIRS}
)

#
# Set target link libraries
#
target_link_libraries(${TARGET_PROJECT_NAME} -Wl,--no-undefined)
target_link_libraries(${TARGET_PROJECT_NAME}
  libcomm_datalayer.so
  pthread
  systemd
  zmq
  ssl
  crypto
)

#
# Set destination paths for installation
#
install( TARGETS ${TARGET_PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  COMPONENT ${BUNDLE_PROJECT_NAME}
) 

if (BUILD_SNAP)
  #
  # Set destination paths for installation
  #
  install( TARGETS ${TARGET_PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/build
    LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/build
    ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/build
    COMPONENT ${BUNDLE_PROJECT_NAME}
  ) 
endif(BUILD_SNAP)

#
# Installation of files
#
add_custom_command(TARGET ${TARGET_PROJECT_NAME}
POST_BUILD
COMMAND make install
)

if (BUILD_SNAP)
  add_custom_command( TARGET ${TARGET_PROJECT_NAME}
    POST_BUILD
    COMMAND snapcraft clean --destructive-mode
    COMMAND snapcraft --enable-experimental-target-arch --target-arch=${SNAP_ARCH} --destructive-mode
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
endif(BUILD_SNAP)