cmake_minimum_required(VERSION 3.16)

project(protocl VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BCOROUTINE "Build library with COROUTINE" ON)

find_package(Boost REQUIRED json system coroutine context)
find_package(Threads REQUIRED)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  find_library(WS2_32_LIBRARY ws2_32)
endif()

add_subdirectory(demo)

add_library(${PROJECT_NAME}
    HttpClient.h
    HttpClient.cpp
    WebscoketClient.h
    WebscoketClient.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Boost::json
    Boost::system
    Boost::coroutine
    Boost::context
    Threads::Threads
    logging
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
  $<$<BOOL:BCOROUTINE>:BCOROUTINE>
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_link_libraries(${PROJECT_NAME}
      PRIVATE ${WS2_32_LIBRARY}
  )
endif()
