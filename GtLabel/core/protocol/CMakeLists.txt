cmake_minimum_required(VERSION 3.16)

project(protocl VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED json)
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
        PRIVATE Boost::json
        Threads::Threads
        logging
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_link_libraries(${PROJECT_NAME}
      PRIVATE ${WS2_32_LIBRARY}
  )
endif()
