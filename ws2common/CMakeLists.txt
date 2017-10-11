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
    ./src/ws2common/MessageHandler.cpp
    ./src/ws2common/Stage.cpp
    ./src/ws2common/scene/SceneNode.cpp
    ./src/ws2common/scene/MeshSceneNode.cpp
    ./src/ws2common/scene/GoalSceneNode.cpp
    ./src/ws2common/scene/BumperSceneNode.cpp
    ./src/ws2common/scene/JamabarSceneNode.cpp
    ./src/ws2common/scene/BananaSceneNode.cpp
    ./src/ws2common/scene/FalloutVolumeSceneNode.cpp
    ./src/ws2common/scene/SwitchSceneNode.cpp
    ./src/ws2common/scene/WormholeSceneNode.cpp
    ./src/ws2common/config/XMLConfigParser.cpp
    )

set(HEADER_FILES
    ./include/ws2common/MessageHandler.hpp
    ./include/ws2common/EnumGameVersion.hpp
    ./include/ws2common/EnumGoalType.hpp
    ./include/ws2common/EnumBananaType.hpp
    ./include/ws2common/EnumPlaybackState.hpp
    ./include/ws2common/Stage.hpp
    ./include/ws2common/scene/SceneNode.hpp
    ./include/ws2common/scene/MeshSceneNode.hpp
    ./include/ws2common/scene/GoalSceneNode.hpp
    ./include/ws2common/scene/BumperSceneNode.hpp
    ./include/ws2common/scene/JamabarSceneNode.hpp
    ./include/ws2common/scene/BananaSceneNode.hpp
    ./include/ws2common/scene/FalloutVolumeSceneNode.hpp
    ./include/ws2common/scene/SwitchSceneNode.hpp
    ./include/ws2common/scene/WormholeSceneNode.hpp
    ./include/ws2common/config/XMLConfigParser.hpp
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

