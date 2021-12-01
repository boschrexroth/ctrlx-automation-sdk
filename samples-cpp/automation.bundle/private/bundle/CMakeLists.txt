
#
# Part of Manifestfile
#
set(BUNDLEX_NAME "com.boschrexroth.sdk.control.bundle")
set(BUNDLEX_DISPLAY_NAME "com_boschrexroth_sdk_control_bundle")
set(BUNDLEX_VERSION ${VERSION}) #Set bei CMakeLists.txt can be modified if more than one bundle is deployed
set(BUNDLEX_DESCRIPTION "ctrlX Motion Bundle example")

#
# Set up source files and header
#
set(ACTIVATOR_NAME ${BUNDLEX_DISPLAY_NAME})

add_library(${ACTIVATOR_NAME} SHARED
  activator.cpp
  cmp1.cpp
)


#
# build dependencies before build activator and bundle
#
add_dependencies( ${ACTIVATOR_NAME} sdk_control )

# don't set RPATH in shared library of activator because possibility wrong build paths in *.so
set_target_properties(${ACTIVATOR_NAME} PROPERTIES SKIP_BUILD_RPATH true)

#
#
#
target_include_directories( ${ACTIVATOR_NAME}
  PRIVATE ${LIBRARY_INCLUDES}
  PRIVATE .
  PRIVATE ./../impl
  PRIVATE ${CMAKE_SOURCE_DIR}/public/include
  PRIVATE ${USER_DEPENDENCY_DIR}/public/include/oss.celix
  PRIVATE ${USER_DEPENDENCY_DIR}/include/motion.core
  PRIVATE ${USER_DEPENDENCY_DIR}/include/motion.lib.base
  PRIVATE ${USER_DEPENDENCY_DIR}/include/motion.lib.common
  PRIVATE ${USER_DEPENDENCY_DIR}/include/motion.lib.jobs
)

target_link_libraries(${ACTIVATOR_NAME} -Wl,--no-undefined -Wl,--whole-archive ${CELIX_DM_STATIC_CXX_LIB} -Wl,--no-whole-archive ${CELIX_LIBRARIES})
target_link_libraries(${ACTIVATOR_NAME}
  # *** Comment / uncomment SHARED libraries of components here ***
  sdk_control
)

add_celix_bundle(${BUNDLEX_NAME}
  SYMBOLIC_NAME ${BUNDLEX_DISPLAY_NAME}
  VERSION ${BUNDLEX_VERSION}
  ACTIVATOR ${ACTIVATOR_NAME}
  DESCRIPTION ${BUNDLEX_DESCRIPTION}
  EXPORT_LIBRARIES
    sdk_control
)

#
# Install target dep. generator type
#
# single-configuration generator (Unix Makefile)
message(STATUS "Bundle directrory: ${BUNDLEX_PUBLIC_INSTALL_DIR}")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${BUNDLEX_NAME}.zip DESTINATION ${BUNDLEX_PUBLIC_INSTALL_DIR}/)




#
# Snapcraft
#
#set( BUNDLEPATH ${CMAKE_CURRENT_BINARY_DIR}/${BUNDLEX_NAME}.zip)
#file(COPY ${CMAKE_SOURCE_DIR}/snapcraft.yaml DESTINATION ${CMAKE_CURRENT_LIST_DIR}/)
#add_custom_command( TARGET ${ACTIVATOR_NAME}
#  POST_BUILD
#  COMMAND snapcraft --target-arch=${SNAP_ARCH}
#  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
#)
