cmake_minimum_required(VERSION 3.9.0)
project(smblevelworkshop2)

#Versioning
set(RELEASE_VERSION_MAJOR 1)
set(RELEASE_VERSION_MINOR 0)
set(RELEASE_VERSION_PATCH 0)

set(RELEASE_IS_PRERELEASE TRUE)
set(RELEASE_VERSION_PRERELEASE_KIND BETA)
set(RELEASE_VERSION_PRERELEASE_BUILD 1)

if(${RELEASE_IS_PRERELEASE})
    string(TOLOWER ${RELEASE_VERSION_PRERELEASE_KIND} LOWER_PRERELEASE_KIND)
    set(RELEASE_VERSION_STRING "${RELEASE_VERSION_MAJOR}.${RELEASE_VERSION_MINOR}.${RELEASE_VERSION_PATCH}-${LOWER_PRERELEASE_KIND}.${RELEASE_VERSION_PRERELEASE_BUILD}")
else(${RELEASE_IS_PRERELEASE})
    set(RELEASE_VERSION_STRING "${RELEASE_VERSION_MAJOR}.${RELEASE_VERSION_MINOR}.${RELEASE_VERSION_PATCH}")
endif(${RELEASE_IS_PRERELEASE})

message(STATUS "smblevelworkshop2 version ${RELEASE_VERSION_STRING}")

if(WIN32)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        #Linking doesn't work so well with Clang with the dllexport/dllimport faff
        message(WARNING "Compiling with Clang on Windows may not work so well - Try gcc if you run into linker errors with __imp__* symbols\n(Oh and please let me know if you can get this working)")
    endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
endif(WIN32)

#Export compile commands for editor autocomplete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Fix some issues with dependencies ignoring rpaths on Linux
if(UNIX)
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-Wl,--disable-new-dtags")
    set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "-Wl,--disable-new-dtags")
endif(UNIX)

#Add uninstall target
configure_file("cmake/uninstall.cmake" "cmake/uninstall.cmake" COPYONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "cmake/uninstall.cmake")

#Add installprerequisites target
set(platformPlugins "")
find_package(Qt5Gui REQUIRED)
foreach(plugin ${Qt5Gui_PLUGINS})
    get_target_property(loc ${plugin} LOCATION)

    list(APPEND platformPlugins ${loc})
endforeach()
list(FILTER platformPlugins INCLUDE REGEX ".*platforms.*") #Only include platform plugins
string (REPLACE ";" "$<SEMICOLON>" platformPluginsStr "${platformPlugins}")

configure_file("cmake/installprerequisites.cmake" "cmake/installprerequisites.cmake" COPYONLY)
add_custom_target(installprerequisites "${CMAKE_COMMAND}" -DCMAKE_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}" -DQT_PLATFORM_PLUGINS="${platformPluginsStr}" -P "cmake/installprerequisites.cmake")

set(TRANSLATIONS
    lang/lang_en_US.ts
    #lang/lang_ja_JA.ts
    )

foreach(val ${TRANSLATIONS})
    message(STATUS "Translation: " ${val})
endforeach(val ${TRANSLATIONS})

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#Set rpath for stuff that gets installed in `lib`
if(APPLE)
    set(CMAKE_INSTALL_RPATH "@loader_path/../lib;@loader_path")
else(APPLE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN")
endif(APPLE)

add_subdirectory(./ws2common)
add_subdirectory(./ws2lz)
add_subdirectory(./ws2lzfrontend)
add_subdirectory(./ws2editor)

#Set rpath for stuff that gets installed in `share/ws2editor/plugins`
if(APPLE)
    set(CMAKE_INSTALL_RPATH "@loader_path/../../../lib;@loader_path")
else(APPLE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../../../lib:$ORIGIN")
endif(APPLE)

#Editor plugins
add_subdirectory(./ws2editorplugins/ws2editorexampleplugin)
add_subdirectory(./ws2editorplugins/gizmosplugin)
add_subdirectory(./ws2editorplugins/rendercolorsplugin)
add_subdirectory(./ws2editorplugins/propertiesproviderplugin)
add_subdirectory(./ws2editorplugins/lzexportproviderplugin)

if(APPLE)
    set(CPACK_GENERATOR Bundle)
    set(CPACK_BUNDLE_NAME "ws2editor")
    set(CPACK_BUNDLE_PLIST ${CMAKE_SOURCE_DIR}/ws2editor/resources/Raw/Info.plist)
    set(CPACK_BUNDLE_ICON ${CMAKE_SOURCE_DIR}/ws2editor/resources/Raw/ws2editor.icns)
endif(APPLE)
include(CPack)

