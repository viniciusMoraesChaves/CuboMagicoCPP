cmake_minimum_required(VERSION 3.16)

project(CuboMagicoCPP VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(CuboMagicoCPP
    src/main.cpp
    src/core/Cube.cpp
    src/core/Cube.hpp
    src/ui/CubeWidget.cpp
    src/ui/CubeWidget.hpp
    src/ui/MainWindow.cpp
    src/ui/MainWindow.hpp
)

target_link_libraries(CuboMagicoCPP PRIVATE Qt6::Widgets)

enable_testing()

add_executable(CubeCoreTests
    tests/CubeCoreTests.cpp
    src/core/Cube.cpp
    src/core/Cube.hpp
)

target_include_directories(CubeCoreTests PRIVATE src)

add_test(NAME CubeCoreTests COMMAND CubeCoreTests)
