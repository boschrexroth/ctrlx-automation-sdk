#
# Set CMAKE version
#
cmake_minimum_required( VERSION 3.9 )

#
# Project information
# Set project, VERSION should be same as snap version
#
set(VERSION 1.0.0)
project( logbook_snap_project VERSION ${VERSION} )

#
# Set TARGET_PROJECT_NAME for each executable (same as snap "app name" =ExeName)
#
set(TARGET_PROJECT_NAME logbook)

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
    set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
    set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
    SET( PLATFORM_NAME "linux-gcc-aarch64")   
    SET( SNAP_ARCH "arm64")
  ELSE()
    MESSAGE(STATUS "Set Platformname to: linux-gcc-x64")
    SET( PLATFORM_NAME "linux-gcc-x64")
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    SET( SNAP_ARCH "amd64")
  ENDIF()
endif()

#
# set source files for each executable
#
set( SOURCE_FILES_LOGBOOK
  ./main.cpp
)


#
# Define Executables to add from project files and their depending source files
#
add_executable( ${TARGET_PROJECT_NAME}
	${SOURCE_FILES_LOGBOOK}
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
# Set target link libraries
#
TARGET_LINK_LIBRARIES ( ${TARGET_PROJECT_NAME}
  systemd
)

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