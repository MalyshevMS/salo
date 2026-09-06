cmake_minimum_required(VERSION 3.16)
project(salo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui WaylandClient)
find_package(LayerShellQt REQUIRED)

add_executable(salo
    main.cpp
    overlaywindow.h
)

target_link_libraries(salo PRIVATE
    Qt6::Widgets
    Qt6::Gui
    Qt6::WaylandClient
    LayerShellQt::Interface
)
