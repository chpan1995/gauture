cmake_minimum_required(VERSION 3.16)

project(ui_dataAnnotation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Gui)

add_library(ui_dataAnnotation)

qt_add_qml_module(ui_dataAnnotation
    URI ui_dataAnnotation
    VERSION 1.0
    QML_FILES
        DataAnnotation.qml
        ImgView.qml
        RightPane.qml
    OUTPUT_DIRECTORY
        ui_dataAnnotation
    SOURCES
    SOURCES DatatypeModel.h
    DatatypeModel.cpp

)

qt_add_resources(${PROJECT_NAME}
    "resource"
    FILES
)

target_include_directories(ui_dataAnnotation
    PRIVATE
      ${CMAKE_SOURCE_DIR}
    INTERFACE
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include>
)

target_link_libraries(ui_dataAnnotation
    PRIVATE Qt6::Quick
)
