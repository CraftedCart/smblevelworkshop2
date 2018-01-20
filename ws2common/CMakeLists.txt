cmake_minimum_required(VERSION 3.9.0)
project(ws2common)

#Use C++ 11
set(CMAKE_CXX_STANDARD 11)

#Export compile commands for editor autocomplete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Be really pedantic!
add_definitions(-Wall -Wextra -pedantic)

#Automatically run moc and rcc when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#External dependencies
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED) #Need Gui for image guff

find_package(glm REQUIRED)
include_directories(SYSTEM ${GLM_INCLUDE_DIRS})

find_package(assimp REQUIRED)
include_directories(SYSTEM ${ASSIMP_INCLUDE_DIRS})
if(APPLE) #For whatever reason ASSIMP_LIBRARIES does not use a full path on macOS
    link_directories(${ASSIMP_LINK_DIRS})
endif(APPLE)

include_directories(./include)
include_directories(${CMAKE_BINARY_DIR})

set(SOURCE_FILES
    ./src/ws2common/WS2Common.cpp
    ./src/ws2common/MessageHandler.cpp
    ./src/ws2common/MathUtils.cpp
    ./src/ws2common/EnumGoalType.cpp
    ./src/ws2common/EnumBananaType.cpp
    ./src/ws2common/EnumPlaybackState.cpp
    ./src/ws2common/Stage.cpp
    ./src/ws2common/CollisionGrid.cpp
    ./src/ws2common/scene/SceneNode.cpp
    ./src/ws2common/scene/StartSceneNode.cpp
    ./src/ws2common/scene/GroupSceneNode.cpp
    ./src/ws2common/scene/BackgroundGroupSceneNode.cpp
    ./src/ws2common/scene/MeshSceneNode.cpp
    ./src/ws2common/scene/GoalSceneNode.cpp
    ./src/ws2common/scene/BumperSceneNode.cpp
    ./src/ws2common/scene/JamabarSceneNode.cpp
    ./src/ws2common/scene/BananaSceneNode.cpp
    ./src/ws2common/scene/FalloutVolumeSceneNode.cpp
    ./src/ws2common/scene/SwitchSceneNode.cpp
    ./src/ws2common/scene/WormholeSceneNode.cpp
    ./src/ws2common/scene/CollisionSceneNode.cpp
    ./src/ws2common/scene/MeshCollisionSceneNode.cpp
    ./src/ws2common/config/XMLConfigParser.cpp
    ./src/ws2common/model/MeshSegment.cpp
    ./src/ws2common/model/ModelLoader.cpp
    ./src/ws2common/resource/AbstractResource.cpp
    ./src/ws2common/resource/ResourceTexture.cpp
    ./src/ws2common/resource/ResourceMesh.cpp
    ./src/ws2common/exception/Exception.cpp
    ./src/ws2common/exception/RuntimeException.cpp
    ./src/ws2common/exception/IOException.cpp
    ./src/ws2common/exception/ModelLoadingException.cpp
    ./src/ws2common/animation/EnumEasing.cpp
    ./src/ws2common/animation/EnumLoopType.cpp
    ./src/ws2common/animation/TransformAnimation.cpp
    )

set(HEADER_FILES
    ./include/ws2common/WS2Common.hpp
    ./include/ws2common/WS2Common.ipp
    ./include/ws2common/AABB.hpp
    ./include/ws2common/MessageHandler.hpp
    ./include/ws2common/MathUtils.hpp
    ./include/ws2common/EnumGameVersion.hpp
    ./include/ws2common/EnumGoalType.hpp
    ./include/ws2common/EnumBananaType.hpp
    ./include/ws2common/EnumPlaybackState.hpp
    ./include/ws2common/EnumAnimationSeesawType.hpp
    ./include/ws2common/Stage.hpp
    ./include/ws2common/CollisionGrid.hpp
    ./include/ws2common/scene/SceneNode.hpp
    ./include/ws2common/scene/StartSceneNode.hpp
    ./include/ws2common/scene/GroupSceneNode.hpp
    ./include/ws2common/scene/BackgroundGroupSceneNode.hpp
    ./include/ws2common/scene/SceneNode.hpp
    ./include/ws2common/scene/MeshSceneNode.hpp
    ./include/ws2common/scene/GoalSceneNode.hpp
    ./include/ws2common/scene/BumperSceneNode.hpp
    ./include/ws2common/scene/JamabarSceneNode.hpp
    ./include/ws2common/scene/BananaSceneNode.hpp
    ./include/ws2common/scene/FalloutVolumeSceneNode.hpp
    ./include/ws2common/scene/SwitchSceneNode.hpp
    ./include/ws2common/scene/WormholeSceneNode.hpp
    ./include/ws2common/scene/CollisionSceneNode.hpp
    ./include/ws2common/scene/MeshCollisionSceneNode.hpp
    ./include/ws2common/config/XMLConfigParser.hpp
    ./include/ws2common/model/Vertex.hpp
    ./include/ws2common/model/MeshSegment.hpp
    ./include/ws2common/model/ModelLoader.hpp
    ./include/ws2common/model/ModelLoader.ipp
    ./include/ws2common/resource/AbstractResource.hpp
    ./include/ws2common/resource/ResourceTexture.hpp
    ./include/ws2common/resource/ResourceMesh.hpp
    ./include/ws2common/exception/Exception.hpp
    ./include/ws2common/exception/RuntimeException.hpp
    ./include/ws2common/exception/IOException.hpp
    ./include/ws2common/exception/ModelLoadingException.hpp
    ./include/ws2common/animation/EnumEasing.hpp
    ./include/ws2common/animation/EnumLoopType.hpp
    ./include/ws2common/animation/Keyframe.hpp
    ./include/ws2common/animation/Keyframe.ipp
    ./include/ws2common/animation/TransformAnimation.hpp
    )

set(RESOURCE_FILES
    ./resources/ws2common/WS2CommonResources.qrc
    )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})

#External dependencies
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    ${ASSIMP_LIBRARIES}
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

