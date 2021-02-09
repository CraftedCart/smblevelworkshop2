cmake_minimum_required(VERSION 3.9.0)
project(ws2editor)

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
set_source_files_properties(./src/ws2editor/ui/AboutWindow.cpp PROPERTIES COMPILE_FLAGS "-DSMBLEVELWORKSHOP2_COMPILE_TIMESTAMP=${COMPILE_TIMESTAMP} -DSMBLEVELWORKSHOP2_VERSION_STRING=${RELEASE_VERSION_STRING}")

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
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOURCE_FILES
    ./src/ws2editor/main.cpp
    ./src/ws2editor/ui/StageEditorWindow.cpp
    ./src/ws2editor/ui/AboutWindow.cpp
    ./src/ws2editor/ui/PluginsWindow.cpp
    ./src/ws2editor/ui/StageIdeaGeneratorWindow.cpp
    ./src/ws2editor/ui/SettingsDialog.cpp
    ./src/ws2editor/ui/CommandWidget.cpp
    ./src/ws2editor/ui/StatusPopupWidget.cpp
    ./src/ws2editor/widget/ViewportWidget.cpp
    ./src/ws2editor/widget/OutlinerWidget.cpp
    ./src/ws2editor/widget/PropertiesWidget.cpp
    ./src/ws2editor/widget/DraggableDoubleSpinBox.cpp
    ./src/ws2editor/widget/Vec3DraggableSpinBoxes.cpp
    ./src/ws2editor/widget/CollapsableWidget.cpp
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
    ./src/ws2editor/physics/PhysicsUtils.cpp
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
    ./src/ws2editor/IExportProvider.cpp
    )

set(HEADER_FILES
    ./include/ws2editor/ui/StageEditorWindow.hpp
    ./include/ws2editor/ui/AboutWindow.hpp
    ./include/ws2editor/ui/PluginsWindow.hpp
    ./include/ws2editor/ui/StageIdeaGeneratorWindow.hpp
    ./include/ws2editor/ui/SettingsDialog.hpp
    ./include/ws2editor/ui/CommandWidget.hpp
    ./include/ws2editor/ui/StatusPopupWidget.hpp
    ./include/ws2editor/widget/ViewportWidget.hpp
    ./include/ws2editor/widget/OutlinerWidget.hpp
    ./include/ws2editor/widget/PropertiesWidget.hpp
    ./include/ws2editor/widget/DraggableDoubleSpinBox.hpp
    ./include/ws2editor/widget/Vec3DraggableSpinBoxes.hpp
    ./include/ws2editor/widget/CollapsableWidget.hpp
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
    ./include/ws2editor/physics/PhysicsUtils.hpp
    ./include/ws2editor/physics/PhysicsUtils.ipp
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
    ./include/ws2editor/IExportProvider.hpp
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

#The bulk of ws2editor will be in a library as trying to get plugins to work on macOS/Windows with an executable was just pain
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} ${TRANSLATION_FILES})
generate_export_header(${PROJECT_NAME})

#External dependencies
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    ${OPENGL_LIBRARIES}
    GLEW::GLEW #Whyyyyyy... so ${GLEW_LIBRARIES} stopped working for whatever reason so we have to use GLEW::GLEW now
    ${BULLET_LIBRARIES}
    ws2common
    )

#Export headers
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

#The launcher for ws2editor
add_executable(${PROJECT_NAME}launcher launcher/main.cpp)
target_link_libraries(${PROJECT_NAME}launcher ws2editor)
set_target_properties(${PROJECT_NAME}launcher PROPERTIES OUTPUT_NAME ws2editor)

if(WIN32)
    #Windows has no concept of rpath, so just group all the exes/dlls in one big mess of a bin directory
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
else(WIN32)
    install(TARGETS ${PROJECT_NAME} DESTINATION lib)
endif(WIN32)

install(TARGETS ${PROJECT_NAME}launcher DESTINATION bin)

#.desktop and icon files
if(UNIX AND NOT APPLE)
    install(FILES ./resources/Raw/ws2editor.desktop DESTINATION share/applications)
    install(FILES ./resources/Raw/ws2editor.png DESTINATION share/icons)
endif(UNIX AND NOT APPLE)

#Translations
install(FILES ${QM_FILES} DESTINATION share/ws2editor/lang)

