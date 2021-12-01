# --- example for shared library ---
set( TARGET_PROJECT_NAME_SHARED sdk_trace )

add_library( ${TARGET_PROJECT_NAME_SHARED} SHARED
  ./../impl/hello_trace.cpp
)

target_include_directories( ${TARGET_PROJECT_NAME_SHARED}
  PRIVATE ${LIBRARY_INCLUDES}
  PRIVATE ./../impl
  PRIVATE ${USER_DEPENDENCY_DIR}/include/common.log.trace
)

target_link_libraries(${TARGET_PROJECT_NAME_SHARED}
    #systemd
    common_log_trace_buffered_static
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


