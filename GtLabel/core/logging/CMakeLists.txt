cmake_minimum_required(VERSION 3.5)

project(logging)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(spdlog)

add_library(${PROJECT_NAME}
  log.cpp
  log.h
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    spdlog::spdlog
    $<$<BOOL:${MINGW}>:ws2_32>
)

# add_executable(demo_log EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/demo/demo_log.cpp)
# target_link_libraries(demo_log PRIVATE ${PROJECT_NAME})
