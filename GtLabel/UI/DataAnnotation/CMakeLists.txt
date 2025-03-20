cmake_minimum_required(VERSION 3.16)

project(ui_dataAnnotation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Quick Gui Widgets)
find_package(Boost REQUIRED json)

set(QML_IMPORT_PATH ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "" FORCE)
set(QML_IMPORT_PATH_DEVELOPER ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "" FORCE)

add_library(ui_dataAnnotation)

qt_add_qml_module(ui_dataAnnotation
    URI ui_dataAnnotation
    VERSION 1.0
    QML_FILES
        DataAnnotation.qml
        ImgView.qml
        RightPane.qml
        TaskInfo.qml
        TagPane.qml
        componentCreationTagPane.js
    OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/ui_dataAnnotation # import 能找到
    SOURCES
    DatatypeModel.h
    DatatypeModel.cpp
    LabelImgData.h
    LabelImgData.cpp
    TaskInfoModel.h
    TaskInfoModel.cpp
    TaskInfoItem.h
    TaskInfoItem.cpp
    LabelTags.h
    LabelTags.cpp
    SOURCES
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
    PRIVATE Qt6::Quick Qt6::Widgets Boost::json
    protocl
    common
    logging
)
