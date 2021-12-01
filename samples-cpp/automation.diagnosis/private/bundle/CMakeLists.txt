
#
# Part of Manifestfile
#
set(BUNDLEX_NAME "com.boschrexroth.sdk.diagnosis.bundle")
set(BUNDLEX_DISPLAY_NAME "com_boschrexroth_sdk_diagnosis_bundle")
set(BUNDLEX_VERSION ${VERSION}) #Set bei CMakeLists.txt can be modified if more than one bundle is deployed
set(BUNDLEX_DESCRIPTION "ctrlX CORE Bundle")

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
add_dependencies( ${ACTIVATOR_NAME} sdk_diagnosis )

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
  PRIVATE ${USER_DEPENDENCY_DIR}/include/common.log.diagnosis
)

target_link_libraries(${ACTIVATOR_NAME} -Wl,--no-undefined -Wl,--whole-archive ${CELIX_DM_STATIC_CXX_LIB} -Wl,--no-whole-archive ${CELIX_LIBRARIES})
target_link_libraries(${ACTIVATOR_NAME}
  # *** Comment / uncomment SHARED libraries of components here ***
  sdk_diagnosis
)

add_celix_bundle(${BUNDLEX_NAME}
  SYMBOLIC_NAME ${BUNDLEX_DISPLAY_NAME}
  VERSION ${BUNDLEX_VERSION}
  ACTIVATOR ${ACTIVATOR_NAME}
  DESCRIPTION ${BUNDLEX_DESCRIPTION}
  EXPORT_LIBRARIES
    sdk_diagnosis
)


#
# Copy resources: JSON file including own main and detailed diagnostics
#
celix_bundle_files(${BUNDLEX_NAME}
${CMAKE_SOURCE_DIR}/private/impl/resources/diagnostics
DESTINATION "resources"
)


#
# Install target dep. generator type
#
# single-configuration generator (Unix Makefile)
message(STATUS "Bundle directrory: ${BUNDLEX_PUBLIC_INSTALL_DIR}")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${BUNDLEX_NAME}.zip DESTINATION ${BUNDLEX_PUBLIC_INSTALL_DIR}/)

#
# Create a List with necessary bundles
#
set(BUNDLE_LIST "")
file(GLOB_RECURSE BUNDLES_CX ${CMAKE_SOURCE_DIR}/../../bundle/*/linux-gcc-x64/*.zip)
foreach(BUNDLE_CX ${BUNDLES_CX})
  list(APPEND BUNDLE_LIST ${BUNDLE_CX})
endforeach()

set(BUNDLE_LIST_FW "")
file(GLOB_RECURSE BUNDLES_CX_FW ${CMAKE_SOURCE_DIR}/../../bin/framework/linux-gcc-x64/bundle/*.zip)
foreach(BUNDLE_CX_FW ${BUNDLES_CX_FW})
  list(APPEND BUNDLE_LIST_FW ${BUNDLE_CX_FW})
endforeach()


#
# bundle list will be modified by bundles in sub directories (above, search and replace)
#
foreach(BUNDLE_CX_TMP ${BUNDLES_CX_FW})
  list(APPEND BUNDLE_LIST ${BUNDLE_CX_TMP})
endforeach()
message(BUNDLE_LIST= ${BUNDLE_LIST})
message(BUNDLE_LIST_FW= ${BUNDLE_LIST_FW})
list(APPEND BUNDLE_LIST ${BUNDLEX_PUBLIC_INSTALL_DIR}/${BUNDLEX_NAME}.zip)

string(REPLACE ";" " " START_LIST "${BUNDLE_LIST}")
file(GENERATE
     OUTPUT "${CMAKE_SOURCE_DIR}/generated/config.properties"
     CONTENT "cosgi.auto.start.1=${START_LIST}\n"
)