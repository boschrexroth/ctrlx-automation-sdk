add_executable(provider)
target_sources(provider PRIVATE 
              ./main.cpp
              ./clientpublisher.cpp)

#
# Set target link libraries
#
target_link_libraries(provider -Wl,--no-undefined)
target_link_libraries(provider
  libcomm_datalayer.so
  pthread
  systemd
)

#
# Set destination paths for installation
#
install( TARGETS provider
  RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  COMPONENT ${BUNDLE_PROJECT_NAME}
)


