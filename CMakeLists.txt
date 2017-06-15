cmake_minimum_required(VERSION 3.8.1)
project(smblevelworkhop2)

#Use C++ 11
set(CMAKE_CXX_STANDARD 11)

#Export compile commands for editor autocomplete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Automatically run moc, uic and rcc when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#External dependencies
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

include_directories(./include)

set(SOURCE_FILES
    ./src/main.cpp
    ./src/StageEditorWindow.cpp
    ./src/widget/ViewportWidget.cpp
    ./src/GLManager.cpp
    ./src/MathUtils.cpp)

set(HEADER_FILES
    ./include/StageEditorWindow.hpp
    ./include/widget/ViewportWidget.hpp
    ./include/GLManager.hpp
    ./include/EnumCameraNav.hpp
    ./include/MathUtils.hpp)

set(RESOURCE_FILES
    ./resources/Workshop2/Workshop2Resources.qrc
    ./resources/Styles/FlatDark/flatdark.qrc)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})

#External dependencies
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES})

