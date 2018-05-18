cmake_minimum_required(VERSION 3.9.0)
project(ws2editor)

#Use C++ 11
set(CMAKE_CXX_STANDARD 11)

#Export compile commands for editor autocomplete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Be really pedantic!
add_definitions(-Wall -Wextra -pedantic)

#Show as an executable, not a shared library in file managers
if(UNIX)
    #-nopie is unused with AppleClang
    if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
    endif(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
endif(UNIX)

#Add compile time proprocessor macro
string(TIMESTAMP COMPILE_TIMESTAMP UTC)
#add_definitions(-DCOMPILE_TIMESTAMP=${COMPILE_TIMESTAMP})
#Don't add definitions for every single file, as this will cause a recompile of everything every time
set_source_files_properties(./src/ws2editor/ui/AboutWindow.cpp PROPERTIES COMPILE_FLAGS -DCOMPILE_TIMESTAMP=${COMPILE_TIMESTAMP})

#Automatically run moc, uic and rcc when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#External dependencies
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

find_package(OpenGL REQUIRED)
include_directories(SYSTEM ${OPENGL_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(SYSTEM ${GLEW_INCLUDE_DIRS})

find_package(Bullet REQUIRED)
include_directories(SYSTEM ${BULLET_INCLUDE_DIRS})

include_directories(./include)
include_directories(${CMAKE_BINARY_DIR})

set(SOURCE_FILES
    ./src/ws2editor/main.cpp
    ./src/ws2editor/ui/StageEditorWindow.cpp
    ./src/ws2editor/ui/AboutWindow.cpp
    ./src/ws2editor/ui/PluginsWindow.cpp
    ./src/ws2editor/ui/StageIdeaGeneratorWindow.cpp
    ./src/ws2editor/ui/SettingsDialog.cpp
    ./src/ws2editor/ui/CommandWidget.cpp
    ./src/ws2editor/widget/ViewportWidget.cpp
    ./src/ws2editor/widget/OutlinerWidget.cpp
    ./src/ws2editor/MathUtils.cpp
    ./src/ws2editor/resource/ResourceManager.cpp
    ./src/ws2editor/resource/ResourceScene.cpp
    ./src/ws2editor/project/Project.cpp
    ./src/ws2editor/project/ProjectManager.cpp
    ./src/ws2editor/ui/ModelManager.cpp
    ./src/ws2editor/ui/ModelResources.cpp
    ./src/ws2editor/ui/ModelOutliner.cpp
    ./src/ws2editor/WS2EditorInstance.cpp
    ./src/ws2editor/scene/SceneSelectionManager.cpp
    ./src/ws2editor/physics/PhysicsManager.cpp
    ./src/ws2editor/physics/PhysicsContainer.cpp
    ./src/ws2editor/PhysicsDebugDrawer.cpp
    ./src/ws2editor/Config.cpp
    ./src/ws2editor/Progress.cpp
    ./src/ws2editor/task/Task.cpp
    ./src/ws2editor/task/TaskManager.cpp
    ./src/ws2editor/task/ImportFileTask.cpp
    ./src/ws2editor/task/LoadGlTextureTask.cpp
    ./src/ws2editor/RenderManager.cpp
    ./src/ws2editor/CachedGlMesh.cpp
    ./src/ws2editor/CachedGlTexture.cpp
    ./src/ws2editor/rendering/MeshRenderCommand.cpp
    ./src/ws2editor/rendering/DebugRenderCommand.cpp
    ./src/ws2editor/MeshNodeData.cpp
    ./src/ws2editor/command/CommandInterpreter.cpp
    ./src/ws2editor/command/CommandReply.cpp
    ./src/ws2editor/command/ListCommandsCommand.cpp
    ./src/ws2editor/command/QuitCommand.cpp
    ./src/ws2editor/command/RestartCommand.cpp
    ./src/ws2editor/command/TranslateSelectedCommand.cpp
    ./src/ws2editor/command/ToggleDebugDrawingCommand.cpp
    )

set(HEADER_FILES
    ./include/ws2editor/ui/StageEditorWindow.hpp
    ./include/ws2editor/ui/AboutWindow.hpp
    ./include/ws2editor/ui/PluginsWindow.hpp
    ./include/ws2editor/ui/StageIdeaGeneratorWindow.hpp
    ./include/ws2editor/ui/SettingsDialog.hpp
    ./include/ws2editor/ui/CommandWidget.hpp
    ./include/ws2editor/widget/ViewportWidget.hpp
    ./include/ws2editor/widget/OutlinerWidget.hpp
    ./include/ws2editor/EnumCameraNav.hpp
    ./include/ws2editor/MathUtils.hpp
    ./include/ws2editor/resource/ResourceManager.hpp
    ./include/ws2editor/resource/ResourceManager.ipp
    ./include/ws2editor/resource/ResourceScene.hpp
    ./include/ws2editor/project/Project.hpp
    ./include/ws2editor/project/ProjectManager.hpp
    ./include/ws2editor/ui/ModelManager.hpp
    ./include/ws2editor/ui/ModelResources.hpp
    ./include/ws2editor/ui/ModelOutliner.hpp
    ./include/ws2editor/WS2EditorInstance.hpp
    ./include/ws2editor/scene/SceneSelectionManager.hpp
    ./include/ws2editor/physics/PhysicsManager.hpp
    ./include/ws2editor/physics/PhysicsContainer.hpp
    ./include/ws2editor/PhysicsDebugDrawer.hpp
    ./include/ws2editor/Config.hpp
    ./include/ws2editor/Progress.hpp
    ./include/ws2editor/task/Task.hpp
    ./include/ws2editor/task/TaskManager.hpp
    ./include/ws2editor/task/ImportFileTask.hpp
    ./include/ws2editor/task/LoadGlTextureTask.hpp
    ./include/ws2editor/RenderManager.hpp
    ./include/ws2editor/CachedGlMesh.hpp
    ./include/ws2editor/CachedGlTexture.hpp
    ./include/ws2editor/rendering/IRenderCommand.hpp
    ./include/ws2editor/rendering/MeshRenderCommand.hpp
    ./include/ws2editor/rendering/DebugRenderCommand.hpp
    ./include/ws2editor/MeshNodeData.hpp
    ./include/ws2editor/command/CommandInterpreter.hpp
    ./include/ws2editor/command/CommandReply.hpp
    ./include/ws2editor/command/ICommandHandler.hpp
    ./include/ws2editor/command/ListCommandsCommand.hpp
    ./include/ws2editor/command/QuitCommand.hpp
    ./include/ws2editor/command/RestartCommand.hpp
    ./include/ws2editor/command/TranslateSelectedCommand.hpp
    ./include/ws2editor/command/ToggleDebugDrawingCommand.hpp
    ./include/ws2editor/plugin/IEditorPlugin.hpp
    )

set(UI_FILES
    ./src/ws2editor/ui/StageEditorWindow.ui
    ./src/ws2editor/ui/SettingsDialog.ui
    ./src/ws2editor/ui/StageIdeaGeneratorWindow.ui
    ./src/ws2editor/ui/AboutWindow.ui
    )

set(RESOURCE_FILES
    ./resources/Workshop2/Workshop2Resources.qrc
    ./resources/Styles/FlatDark/flatdark.qrc
    )

#TRANSLATIONS is defined by the parent CMakeLists.txt
qt5_create_translation(TRANSLATION_FILES ${SOURCE_FILES} ${HEADER_FILES} ${UI_FILES} ${TRANSLATIONS})
qt5_add_translation(QM_FILES ${TRANSLATIONS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} ${TRANSLATION_FILES})

#External dependencies
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${BULLET_LIBRARIES}
    ws2common
    )

#Export headers
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

#Enable symbol exporting for plugins
set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS 1)
#I don't think this actually does anything except on the MSVC compiler
#set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS 1)

if(WIN32)
    #Invoke ar to generate a .dll.a from the .obj files, required to link plugins
    add_custom_command(TARGET ${PROJECT_NAME} PRE_LINK
        COMMAND sh -c "${CMAKE_AR} cr lib${PROJECT_NAME}.dll.a $$(find . -name '*.obj' -printf '%p ')"
        COMMENT "Generating lib${PROJECT_NAME}.dll.a for external linking"
        )

    #Also add the install command for libws2editor.dll.a
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.dll.a DESTINATION bin)
endif(WIN32)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

#.desktop and icon files
if(UNIX AND NOT APPLE)
    install(FILES ./resources/Raw/ws2editor.desktop DESTINATION share/applications)
    install(FILES ./resources/Raw/ws2editor.png DESTINATION share/icons)
endif(UNIX AND NOT APPLE)

#Translations
install(FILES ${QM_FILES} DESTINATION share/ws2editor/lang)

