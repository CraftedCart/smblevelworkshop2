cmake_minimum_required(VERSION 3.9.0)
project(ws2lz)

include(GenerateExportHeader)

#Use C++ 11
set(CMAKE_CXX_STANDARD 11)

#Export compile commands for editor autocomplete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Well if I gotta do it for Windows I may as well do it for other platforms so I don't stuff up the Windows build
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

#Be really pedantic!
add_definitions(-Wall -Wextra -pedantic)

#Automatically run moc when needed
set(CMAKE_AUTOMOC ON)

#External dependencies
find_package(Qt5Core REQUIRED)

find_package(glm REQUIRED)
include_directories(SYSTEM ${GLM_INCLUDE_DIRS})

include_directories(./include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOURCE_FILES
    ./src/ws2lz/WS2Lz.cpp
    ./src/ws2lz/SMB1LzExporter.cpp
    ./src/ws2lz/SMB2LzExporter.cpp
    ./src/ws2lz/TriangleIntersectionGrid.cpp
    ./src/ws2lz/LZSSDictionary.cpp
    ./src/ws2lz/LZCompressor.cpp
    )

set(HEADER_FILES
    ./include/ws2lz/WS2Lz.hpp
    ./include/ws2lz/SMB1LzExporter.hpp
    ./include/ws2lz/SMB2LzExporter.hpp
    ./include/ws2lz/TriangleIntersectionGrid.hpp
    ./include/ws2lz/LZSSDictionary.hpp
    ./include/ws2lz/LZCompressor.hpp
    )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})
generate_export_header(${PROJECT_NAME})

#External dependencies
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    ws2common
    )

#Export headers
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

if(WIN32)
    #Windows has no concept of rpath, so just group all the exes/dlls in one big mess of a bin directory
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
else(WIN32)
    install(TARGETS ${PROJECT_NAME} DESTINATION lib)
endif(WIN32)

