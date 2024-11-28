add_executable(client)
target_sources(client PRIVATE 
               ./main.cpp 
               ./datalayerclientsub.cpp)

#
# Set target link libraries
#
target_link_libraries(client -Wl,--no-undefined)
target_link_libraries(client
  libcomm_datalayer.so
  pthread
  systemd
)

#
# Set destination paths for installation
#
install( TARGETS client
  RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/generated/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
  COMPONENT ${BUNDLE_PROJECT_NAME}
)


