#
# Set CMAKE version
#
cmake_minimum_required( VERSION 3.9 )

#
# Project information
# Set project, VERSION should be same as snap version
#
set(VERSION 1.0.0)
project( datalayer_client_snap_project VERSION ${VERSION} )

#
# Set TARGET_PROJECT_NAME for each executable (same as snap "app name" =ExeName)
#
set(TARGET_PROJECT_NAME datalayerClient)

#
# User dependency directories
#
set (USER_DEPENDENCY_DIR ${CMAKE_CURRENT_LIST_DIR}/../..)
set (DATALAYER_DEP_DIR ${USER_DEPENDENCY_DIR}/bin/comm.datalayer/)
set (ZMQ_DEP_DIR ${USER_DEPENDENCY_DIR}/bin/zmq/)

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
    MESSAGE(STATUS "Set Platformname to: linux-gcc-aarch64")
    SET(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
    SET(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
    SET(PLATFORM_NAME "linux-gcc-aarch64")
    SET(SNAP_ARCH "arm64")
  ELSE()
    MESSAGE(STATUS "Set Platformname to: linux-gcc-x64")
    SET(CMAKE_C_COMPILER gcc)
    SET(CMAKE_CXX_COMPILER g++)
    SET(PLATFORM_NAME "linux-gcc-x64")
    SET(SNAP_ARCH "amd64")
  ENDIF()
endif()

#
# set source files for each executable
#
set( SOURCE_FILES_DATALAYERCLIENT
  ./main.cpp
  ./datalayerclient.cpp
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
set (FLATBUFFERS_FLATC_EXECUTABLE ${USER_DEPENDENCY_DIR}/bin/oss.flatbuffers/linux-gcc-x64/release/flatc)
set (FLATBUFFERS_INCLUDE_DIR ${USER_DEPENDENCY_DIR}/include/oss.flatbuffers/oss.flatbuffers)
#include (${USER_DEPENDENCY_DIR}/cmake/oss.flatbuffers/FindFlatBuffers.cmake)
set( FLATBUFFERS_FLATC_SCHEMA_EXTRA_ARGS "--gen-object-api")

# build_flatbuffers("sampleSchema.fbs" "" "datalayerclientflatbuffers" "${CMAKE_CURRENT_LIST_DIR}" "${CMAKE_CURRENT_LIST_DIR}" "${CMAKE_CURRENT_LIST_DIR}/bfbs" "")

#
# Set link directories
#
MESSAGE( STATUS "Libraries directory: ${LIBRARY_DIR}")

if( UNIX )
  IF( ${CMAKE_BUILD_KIT} MATCHES "aarch64")
  link_directories(
    ${LIBRARY_DIR} 
    ${LIBRARY_DEP_DIR}
    )
  ELSE()
  link_directories(
    C:/git/sdk.control/public/samples-cpp/datalayer.client/../../bin/comm.datalayer/win-msvc-x86/
    ${ZMQ_DEP_DIR}/win-msvc-x86/release/
    ${LIBRARY_DIR} 
    ${LIBRARY_DEP_DIR}
    )
  ENDIF()
endif()


#
# Define Executables to add from project files and their depending source files
#
add_executable( ${TARGET_PROJECT_NAME}
	${SOURCE_FILES_DATALAYERCLIENT}
  )

#add_dependencies(${TARGET_PROJECT_NAME} datalayerclientflatbuffers)

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
IF (UNIX)
TARGET_LINK_LIBRARIES ( ${TARGET_PROJECT_NAME}
  ${DATALAYER_DEP_DIR}/${PLATFORM_NAME}/release/libcomm_datalayer.so
  pthread
  systemd
  ${ZMQ_DEP_DIR}/${PLATFORM_NAME}/release/libzmq.so
  ssl
  crypto
)
ELSE ()
message(STATUS "++++++++++++++++++${DATALAYER_DEP_DIR}win-msvc-x86/")
TARGET_LINK_LIBRARIES ( ${TARGET_PROJECT_NAME}
  C:/git/sdk.control/public/samples-cpp/datalayer.client/../../bin/comm.datalayer/win-msvc-x86/release/comm_datalayer.dll
  ${ZMQ_DEP_DIR}/win-msvc-x86/release/libzmq.dll
)
ENDIF()
#
# Set destination paths for installation
#
install( TARGETS ${TARGET_PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  COMPONENT ${BUNDLE_PROJECT_NAME}
) 

#
# Set destination paths of user dependency files for installation
#
install(FILES ${DATALAYER_DEP_DIR}/${PLATFORM_NAME}/release/libcomm_datalayer.so DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}/usr/lib)
install(FILES ${ZMQ_DEP_DIR}/${PLATFORM_NAME}/release/libzmq.so DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}/usr/lib)
install(FILES ${ZMQ_DEP_DIR}/${PLATFORM_NAME}/release/libzmq.so.5 DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}/usr/lib)

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

  #
  # Set destination paths of user dependency files for installation
  #
  install(FILES ${DATALAYER_DEP_DIR}/${PLATFORM_NAME}/release/libcomm_datalayer.so DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/build/usr/lib)
  install(FILES ${ZMQ_DEP_DIR}/${PLATFORM_NAME}/release/libzmq.so DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/build/usr/lib)
  install(FILES ${ZMQ_DEP_DIR}/${PLATFORM_NAME}/release/libzmq.so.5 DESTINATION ${CMAKE_CURRENT_LIST_DIR}/generated/build/usr/lib)
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
    COMMAND snapcraft --target-arch=${SNAP_ARCH}
    COMMAND snapcraft clean
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
endif(BUILD_SNAP)