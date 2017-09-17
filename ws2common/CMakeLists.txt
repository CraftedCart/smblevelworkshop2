cmake_minimum_required(VERSION 3.6.2)
project(ws2common)

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

include_directories(./include)
include_directories(${CMAKE_BINARY_DIR})

set(SOURCE_FILES
    ./src/MessageHandler.cpp
    ./src/scene/SceneNode.cpp
    ./src/scene/MeshSceneNode.cpp
    ./src/scene/GoalSceneNode.cpp
    ./src/scene/BumperSceneNode.cpp
    ./src/scene/JamabarSceneNode.cpp
    ./src/scene/BananaSceneNode.cpp
    ./src/scene/FalloutVolumeSceneNode.cpp
    ./src/scene/SwitchSceneNode.cpp
    ./src/scene/WormholeSceneNode.cpp
    )

set(HEADER_FILES
    ./include/MessageHandler.hpp
    ./include/EnumGameVersion.hpp
    ./include/EnumGoalType.hpp
    ./include/EnumBananaType.hpp
    ./include/EnumPlaybackState.hpp
    ./include/scene/SceneNode.hpp
    ./include/scene/MeshSceneNode.hpp
    ./include/scene/GoalSceneNode.hpp
    ./include/scene/BumperSceneNode.hpp
    ./include/scene/JamabarSceneNode.hpp
    ./include/scene/BananaSceneNode.hpp
    ./include/scene/FalloutVolumeSceneNode.hpp
    ./include/scene/SwitchSceneNode.hpp
    ./include/scene/WormholeSceneNode.hpp
    )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})

#External dependencies
target_link_libraries(${PROJECT_NAME} Qt5::Core)

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

