#****************************************************************************************
# Scheduler Remote Client
#****************************************************************************************
set(TARGET_PROJECT_NAME_CLIENT scheduler.remote)

add_executable(${TARGET_PROJECT_NAME_CLIENT}
  main.cpp
  app_client.cpp
  Trace.cpp
  common.log.trace/trace_itf_wrapper.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CTRLX_DL REQUIRED ctrlx-datalayer)
pkg_check_modules(CTRLX_SCHEDULER REQUIRED ctrlx-scheduler)

target_include_directories(${TARGET_PROJECT_NAME_CLIENT}
  PUBLIC
    ${CTRLX_DL_INCLUDE_DIRECTORIES}
    ${CTRLX_SCHEDULER_INCLUDE_DIRECTORIES}
  PRIVATE
    ${SDK_ROOT_DIR}/include/common.log.trace 
    common.log.trace 
)

target_link_directories(${TARGET_PROJECT_NAME_CLIENT} PUBLIC
  ${SDK_ROOT_DIR}/lib/common.log.trace/${PLATFORM_NAME}
)

target_link_libraries(${TARGET_PROJECT_NAME_CLIENT}
  ${CTRLX_DL_LIBRARIES}
  ${CTRLX_SCHEDULER_LIBRARIES}
  ${SDK_ROOT_DIR}/lib/common.log.trace/${PLATFORM_NAME}/release/libcommon_log_trace_buffered_static.a
  systemd
)

if (BUILD_SNAP)
  #
  # Set destination paths for installation
  #
  install( TARGETS ${TARGET_PROJECT_NAME_CLIENT} CONFIGURATIONS ${CMAKE_BUILD_TYPE}
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/generated/build
    LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/generated/build
    ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/generated/build
  )
else()
  install(TARGETS ${TARGET_PROJECT_NAME_CLIENT} CONFIGURATIONS ${CMAKE_BUILD_TYPE}
    ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
    LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  )
endif(BUILD_SNAP)