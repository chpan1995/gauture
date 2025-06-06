﻿cmake_minimum_required(VERSION 3.16)

project(ui_main VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup(REQUIRES 6.5)

find_package(Qt6 REQUIRED COMPONENTS Quick Gui Network)
find_package(Threads REQUIRED)

add_subdirectory(DataAnnotation)

add_library(ui_main)

# 添加这行来设置 QML 策略 common 也会生成qmldir
# 此策略的新行为是，对于模块中包含 QML 文件的每个目录
# 都会生成一个单独的 qmldir 文件
qt_policy(SET QTP0004 NEW)

# Add QML import paths for IDE support
set(QML_IMPORT_PATH ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "" FORCE)
set(QML_IMPORT_PATH_DEVELOPER ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "" FORCE)

qt_add_qml_module(ui_main
    URI ui_main
    VERSION 1.0
    QML_FILES
        Main.qml
        Titlebar.qml
        common/ButtonImg.qml
        common/ButtonImgText.qml
        common/GCombox.qml
        common/ButtonText.qml
        common/ButtonComplex.qml
        common/GProgressBar.qml
        common/GMessageBox.qml
    OUTPUT_DIRECTORY
        ui_main
    SOURCES
    FrameLessView.h
    FrameLessView.cpp
    Login.h
    Login.cpp
)

qt_add_resources(${PROJECT_NAME}
    "images" # resourcename
    FILES
    images/logo.png
    images/connect.png
    images/quit.png
    images/user.png
    images/dataBz.png
    images/dataSelectBz.png
    images/upload.png
    images/requireData.png
    images/tail.png
    images/front10.png
    images/front.png
    images/next.png
    images/next10.png
    images/end.png
    images/down.png
    images/selected.png
    images/fold.png
    images/dashedline.png
    images/max.png
    images/min.png
    images/normal.png
    images/close.png
    images/delete.png
    images/icon.png
    images/logintp.png
    images/loginbtm.png
    images/check.png
    images/checked.png
    images/closedseletd.png
    images/mseled.png
    images/munseleted.png
    images/loginlogo.png
    FILES
    qss/login.qss
)

target_include_directories(ui_main
    PRIVATE
      ${CMAKE_SOURCE_DIR}
    INTERFACE
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include>
)

target_link_libraries(ui_main
    PRIVATE Qt6::Quick Qt6::Gui Qt6::Widgets Threads::Threads Qt6::Network
    ui_dataAnnotation ui_dataAnnotationplugin Boost::json common protocl
    logging
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(ui_main PRIVATE  X11::X11)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(ui_main PRIVATE  dwmapi)
endif()

