cmake_minimum_required(VERSION 3.9.0)
project(lzexportproviderplugin)

#Use C++ 11
set(CMAKE_CXX_STANDARD 11)

#Export compile commands for editor autocomplete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Be really pedantic!
add_definitions(-Wall -Wextra -pedantic)

#Automatically run moc when needed
set(CMAKE_AUTOMOC ON)

#External dependencies
find_package(Qt5Core REQUIRED)

find_package(Bullet REQUIRED)
include_directories(SYSTEM ${BULLET_INCLUDE_DIRS})

include_directories(./include)
include_directories(${CMAKE_BINARY_DIR})

set(SOURCE_FILES
    ./src/lzexportproviderplugin/Plugin.cpp
    ./src/lzexportproviderplugin/SMB2RawLzExportProvider.cpp
    )

set(HEADER_FILES
    ./include/lzexportproviderplugin/Plugin.hpp
    ./include/lzexportproviderplugin/SMB2RawLzExportProvider.hpp
    )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})

#External dependencies
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    ws2editor
    ws2lz
    ${BULLET_LIBRARIES}
    )

#Export headers
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

install(TARGETS ${PROJECT_NAME} DESTINATION share/ws2editor/plugins)

