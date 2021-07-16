# --- example for shared library ---
set( TARGET_PROJECT_NAME_SHARED sdk_control )

add_library( ${TARGET_PROJECT_NAME_SHARED} SHARED
  ./../impl/hello_world.cpp
  ./../impl/hello_motion.cpp
)

message( STATUS "MOTION_DEPENDENCY_DIR=${MOTION_DEPENDENCY_DIR}" )

target_include_directories( ${TARGET_PROJECT_NAME_SHARED}
  PRIVATE ${LIBRARY_INCLUDES}
  PRIVATE ./../impl
  PRIVATE ./../impl/linux
  PRIVATE ${USER_DEPENDENCY_DIR}/include/motion.core
  PRIVATE ${USER_DEPENDENCY_DIR}/include/motion.lib.common
  PRIVATE ${USER_DEPENDENCY_DIR}/include/motion.lib.commands
)

# don't set RPATH in shared library of activator because possibility wrong build paths in *.so
set_target_properties(${TARGET_PROJECT_NAME_SHARED} PROPERTIES SKIP_BUILD_RPATH true)

# single-configuration generator (Unix Makefile)
install( TARGETS ${TARGET_PROJECT_NAME_SHARED}
  RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  COMPONENT ${BUNDLE_PROJECT_NAME}
)
install(FILES ./../impl/hello_world.h DESTINATION ${CMAKE_SOURCE_DIR}/generated/include/sdk/control)

