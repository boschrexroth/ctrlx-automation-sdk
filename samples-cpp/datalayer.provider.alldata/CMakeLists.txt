#
# Set CMAKE version
#
cmake_minimum_required( VERSION 3.9 )

#
# Project information
# Set PROJECT_VERSION - should be same as snap version
#
project( sdk_cpp_datalayer_provider_alldata VERSION 3.0.0 )

#
# Set TARGET_PROJECT_NAME for each executable (same as snap "app name" =ExeName)
#
set(TARGET_PROJECT_NAME allDataProvider)

#
# User dependency directories
#
set (SDK_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../)

#
# Flatbuffers
#
execute_process (COMMAND bash ${CMAKE_CURRENT_LIST_DIR}/build-fbs.sh)

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
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMY_DEBUG")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMY_DEBUG")

#
# set libraries dirs and includes
#
IF( ${BUILD_KIT} MATCHES "aarch64")
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
set( SOURCE_FILES_ALLDATAPROVIDER
  ./main.cpp
  ./providerNodeAllData.cpp
)

#
# set private include directories
#
SET ( PRIVATE_INCLUDE_DIRS
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${SDK_ROOT_DIR}/include
  ${SDK_ROOT_DIR}/include/comm.datalayer
  )


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
	${SOURCE_FILES_ALLDATAPROVIDER}
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
    COMMAND sudo snapcraft clean
    COMMAND sudo snapcraft --build-for=${SNAP_ARCH} --verbosity=verbose
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
endif(BUILD_SNAP)
