cmake_minimum_required(VERSION 3.16)

project(ui_main VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Gui)
find_package(Threads REQUIRED)

add_subdirectory(DataAnnotation)

add_library(ui_main)

qt_add_qml_module(ui_main
    URI ui_main
    VERSION 1.0
    QML_FILES
        Main.qml
        Titlebar.qml
        common/ButtonImg.qml
		common/ButtonImgText.qml
    OUTPUT_DIRECTORY
        ui_main
    SOURCES
    FrameLessView.h
    FrameLessView.cpp
    # RESOURCES
    # images/logo.png
)

qt_add_resources(${PROJECT_NAME}
    "resource"
    FILES
    images/logo.png
    images/connect.png
    images/quit.png
    images/user.png
	images/dataBz.png
	images/dataSelectBz.png
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
	PRIVATE Qt6::Quick Qt6::Gui X11::X11 Threads::Threads ui_dataAnnotation
)
