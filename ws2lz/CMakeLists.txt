cmake_minimum_required(VERSION 3.9.0)
project(ws2lz)

#Use C++ 11
set(CMAKE_CXX_STANDARD 11)

#Export compile commands for editor autocomplete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Be really pedantic!
add_definitions(-Wall -Wextra -pedantic)

#Automatically run moc, uic and rcc when needed
set(CMAKE_AUTOMOC ON)

#External dependencies
find_package(Qt5Core REQUIRED)

find_package(glm REQUIRED)
include_directories(SYSTEM ${GLM_INCLUDE_DIRS})

include_directories(./include)
include_directories(${CMAKE_BINARY_DIR})

set(SOURCE_FILES
    ./src/ws2lz/WS2Lz.cpp
    ./src/ws2lz/SMB2LzExporter.cpp
    ./src/ws2lz/TriangleIntersectionGrid.cpp
    )

set(HEADER_FILES
    ./include/ws2lz/WS2Lz.hpp
    ./include/ws2lz/SMB2LzExporter.hpp
    ./include/ws2lz/TriangleIntersectionGrid.hpp
    )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})

#External dependencies
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    ws2common
    )

#Export headers
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

if(WIN32)
    #Windows has no concept of rpath, so just group all the exes/dlls in one big mess of a directory
    install(TARGETS ${PROJECT_NAME} DESTINATION .)
else(WIN32)
    install(TARGETS ${PROJECT_NAME} DESTINATION lib)
endif(WIN32)

