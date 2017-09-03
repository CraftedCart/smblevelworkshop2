cmake_minimum_required(VERSION 3.6.2)
project(ws2editor)

#Use C++ 11
set(CMAKE_CXX_STANDARD 11)

#Export compile commands for editor autocomplete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Be really pedantic!
add_definitions(-Wall -Wextra -pedantic)

#Add compile time proprocessor macro
string(TIMESTAMP COMPILE_TIMESTAMP UTC)
#add_definitions(-DCOMPILE_TIMESTAMP=${COMPILE_TIMESTAMP})
#Don't add definitions for every single file, as this will cause a recompile of everything every time
set_source_files_properties(./src/ui/AboutWindow.cpp PROPERTIES COMPILE_FLAGS -DCOMPILE_TIMESTAMP=${COMPILE_TIMESTAMP})

#Automatically run moc, uic and rcc when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#External dependencies
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(OpenGL REQUIRED)
include_directories(SYSTEM ${OPENGL_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(SYSTEM ${GLEW_INCLUDE_DIRS})

find_package(assimp REQUIRED)
include_directories(SYSTEM ${ASSIMP_INCLUDE_DIRS})
if(APPLE) #-lassimp doesn't seem to work with homebrew installed Assimp
    set(ASSIMP_LIBRARIES /usr/local/lib/libassimp.dylib)
endif(APPLE)

find_package(Bullet REQUIRED)
include_directories(SYSTEM ${BULLET_INCLUDE_DIRS})

#find_package(ws2common REQUIRED)
#include_directories(SYSTEM ${WS2COMMON_INCLUDE_DIRS})

include_directories(./include)
include_directories(${CMAKE_BINARY_DIR})

set(SOURCE_FILES
    ./src/main.cpp
    ./src/ui/StageEditorWindow.cpp
    ./src/ui/AboutWindow.cpp
    ./src/ui/SettingsDialog.cpp
    ./src/widget/ViewportWidget.cpp
    ./src/widget/OutlinerWidget.cpp
    ./src/GLManager.cpp
    ./src/MathUtils.cpp
    ./src/model/MeshSegment.cpp
    ./src/exception/Exception.cpp
    ./src/exception/RuntimeException.cpp
    ./src/exception/IOException.cpp
    ./src/exception/ModelLoadingException.cpp
    ./src/resource/ResourceManager.cpp
    ./src/resource/AbstractResource.cpp
    ./src/resource/ResourceTexture.cpp
    ./src/resource/ResourceScene.cpp
    ./src/resource/ResourceMesh.cpp
    ./src/project/Project.cpp
    ./src/project/ProjectManager.cpp
    ./src/ui/ModelManager.cpp
    ./src/ui/ModelResources.cpp
    ./src/ui/ModelOutliner.cpp
    ./src/WS2.cpp
    ./src/scene/MeshSceneNode.cpp
    ./src/scene/SceneSelectionManager.cpp
    ./src/physics/PhysicsManager.cpp
    ./src/PhysicsDebugDrawer.cpp
    ./src/Config.cpp
    ./src/Progress.cpp
    ./src/task/TaskManager.cpp
    ./src/task/ImportFileTask.cpp
    )

set(HEADER_FILES
    ./include/ui/StageEditorWindow.hpp
    ./include/ui/AboutWindow.hpp
    ./include/ui/SettingsDialog.hpp
    ./include/widget/ViewportWidget.hpp
    ./include/widget/OutlinerWidget.hpp
    ./include/GLManager.hpp
    ./include/EnumCameraNav.hpp
    ./include/MathUtils.hpp
    ./include/model/Vertex.hpp
    ./include/model/MeshSegment.hpp
    ./include/exception/Exception.hpp
    ./include/exception/RuntimeException.hpp
    ./include/exception/IOException.hpp
    ./include/exception/ModelLoadingException.hpp
    ./include/resource/ResourceManager.hpp
    ./include/resource/AbstractResource.hpp
    ./include/resource/ResourceTexture.hpp
    ./include/resource/ResourceScene.hpp
    ./include/resource/ResourceMesh.hpp
    ./include/project/Project.hpp
    ./include/project/ProjectManager.hpp
    ./include/ui/ModelManager.hpp
    ./include/ui/ModelResources.hpp
    ./include/ui/ModelOutliner.hpp
    ./include/WS2.hpp
    ./include/scene/MeshSceneNode.hpp
    ./include/scene/SceneSelectionManager.hpp
    ./include/physics/PhysicsManger.hpp
    ./include/PhysicsDebugDrawer.hpp
    ./include/Config.hpp
    ./include/Progress.hpp
    ./include/task/Task.hpp
    ./include/task/TaskManager.hpp
    ./include/task/ImportFileTask.hpp
    )

set(RESOURCE_FILES
    ./resources/Workshop2/Workshop2Resources.qrc
    ./resources/Styles/FlatDark/flatdark.qrc
    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})

#External dependencies
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    ${BULLET_LIBRARIES}
    ws2common
    )

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

if(UNIX AND NOT APPLE)
    install(FILES ./resources/Raw/ws2editor.desktop DESTINATION share/applications)
    install(FILES ./resources/Raw/ws2editor.png DESTINATION share/icons)
endif(UNIX AND NOT APPLE)

