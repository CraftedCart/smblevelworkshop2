cmake_minimum_required(VERSION 3.9.0)
project(smblevelworkshop2)

#Export compile commands for editor autocomplete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN")

#Add uninstall target
configure_file("cmake/uninstall.cmake" "cmake/uninstall.cmake" COPYONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "cmake/uninstall.cmake")

#No need to add lang/lang_en_US.ts to the translation list, as the source already has the en_US translation
set(TRANSLATIONS
    lang/lang_en_US.ts
    #lang/lang_ja_JA.ts
    )

foreach(val ${TRANSLATIONS})
    message(STATUS "Translation: " ${val})
endforeach(val ${TRANSLATIONS})

add_subdirectory(./ws2common)
add_subdirectory(./ws2lz)
add_subdirectory(./ws2lzfrontend)
add_subdirectory(./ws2editor)

