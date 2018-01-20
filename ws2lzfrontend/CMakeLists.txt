cmake_minimum_required(VERSION 3.9.0)
project(ws2lzfrontend)

#Use C++ 11
set(CMAKE_CXX_STANDARD 11)

#Export compile commands for editor autocomplete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Be really pedantic!
add_definitions(-Wall -Wextra -pedantic)

#Show as an executable, not a shared library in file managers
if(UNIX)
    #-nopie is usused with AppleClang
    if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
    endif(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
endif(UNIX)

#Automatically run moc, uic and rcc when needed
set(CMAKE_AUTOMOC ON)

#External dependencies
find_package(Qt5Core REQUIRED)
include_directories(./include)
include_directories(${CMAKE_BINARY_DIR})

find_package(glm REQUIRED)
include_directories(SYSTEM ${GLM_INCLUDE_DIRS})

set(SOURCE_FILES
    ./src/ws2lzfrontend/main.cpp
    )

set(HEADER_FILES
    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

#External dependencies
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    ws2common
    ws2lz
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
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
endif(WIN32)

