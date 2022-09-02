cmake_minimum_required(VERSION 3.22)
project(text)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/Users/changguijie/Qt/6.3.1/macos/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        PrintSupport
        REQUIRED)

add_executable(text main.cpp text.cpp text.h showwidget.cpp showwidget.h)
target_link_libraries(text
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::PrintSupport
        )


