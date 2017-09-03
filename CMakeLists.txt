cmake_minimum_required(VERSION 3.6.2)
project(smblevelworkshop2)

#Export compile commands for editor autocomplete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN")

#Add uninstall target
configure_file("cmake/uninstall.cmake" "cmake/uninstall.cmake" COPYONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "cmake/uninstall.cmake")

add_subdirectory(./ws2common)
add_subdirectory(./ws2editor)

