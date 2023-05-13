include(GetPrerequisites)

function(resolve_windows_prereqs var)
    #Darn it I have to resolve Windows paths myself
    if(WIN32)
        set(temp ${WS2EDITOR_PREREQS})
        set(newList "")

        foreach(file ${temp})
            #Iterate over PATH to try and find it
            foreach(pth $ENV{PATH})
                string(TOLOWER pth lowerPth)
                if("${lowerPth}" MATCHES ".*system32.*" OR
                        "${pth}" MATCHES ".*PowerShell.*" OR
                        "${pth}" MATCHES ".*LLVM.*")
                    #Drop System32/PowerShell/LLVM paths
                    continue()
                endif()

                if(EXISTS "${pth}/${file}")
                    list(APPEND newList "${pth}/${file}")
                endif()
            endforeach(pth)
        endforeach(file)

        unset(temp)
        set(${var} ${newList} PARENT_SCOPE)
    endif()
endfunction(resolve_windows_prereqs)

if(WIN32)
    set(BIN_FILES_PATH "${CMAKE_INSTALL_PREFIX}/bin/*.exe")
else(WIN32)
    set(BIN_FILES_PATH "${CMAKE_INSTALL_PREFIX}/bin/*")
endif(WIN32)

if(WIN32)
    set(LIB_FILES_PATH "${CMAKE_INSTALL_PREFIX}/bin/*.dll")
else(WIN32)
    set(LIB_FILES_PATH "${CMAKE_INSTALL_PREFIX}/lib")
endif(WIN32)

if(WIN32)
    #Windows has no concept of rpath, so just group all the exes/dlls in one big mess of a bin directory
    set(LIBPATH "${CMAKE_INSTALL_PREFIX}/bin")
else(WIN32)
    set(LIBPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif(WIN32)

if(WIN32)
    set(EXCLUDE_SYSTEM_LIBS 1)
else(WIN32)
    set(EXCLUDE_SYSTEM_LIBS 0)
endif(WIN32)

message(STATUS "Finding installed files")
file(GLOB INSTALL_FILES ${BIN_FILES_PATH} ${LIB_FILES_PATH})

foreach(installedFile ${INSTALL_FILES})
    if(IS_DIRECTORY ${installedFile})
        continue()
    endif(IS_DIRECTORY ${installedFile})

    get_prerequisites(${installedFile} WS2EDITOR_PREREQS ${EXCLUDE_SYSTEM_LIBS} 1 "" "")
    resolve_windows_prereqs(WS2EDITOR_PREREQS)

    message(STATUS "Installing prerequisites for " ${installedFile})

    #Resolve symlinks
    set(origFiles "")
    foreach(file ${WS2EDITOR_PREREQS})
        message("Copying: " ${file})
        #Drop paths w/ "@rpath" (A macOS thing)
        #I can't be bothered to resolve them and most of the time it shouldn't matter
        if(${file} MATCHES "^@rpath.*")
            message(STATUS "Dropping file " ${file})
            continue()
        endif()

        # Resolve symlink
        get_filename_component(resolvedFile "${file}" REALPATH)
        # Name of resolved symlink file
        get_filename_component(resolvedFileName "${resolvedFile}" NAME)
        # Name of original file
        get_filename_component(origFileName "${file}" NAME)
        # This nonsense is for copying the underlying library while giving it the name of its symlink
        # Why? Well, we want to package this with BlendToSMBStage2, but Blender plugins must be packaged as .ZIP files
        # .ZIP archives don't preserve symlinks, so this hacky nonsense avoids using them
        # Effectively, this should do nothing on platforms like Windows where symlinks aren't an issue
        string(CONCAT expectedFileName ${LIBPATH} "/" ${origFileName})
        string(CONCAT currentFileName ${LIBPATH} "/" ${resolvedFileName})
        file(COPY ${resolvedFile} DESTINATION ${LIBPATH})
        file(RENAME ${currentFileName} ${expectedFileName})
    endforeach(file)
endforeach(installedFile)

#Also fetch the Qt platform plugins
foreach(plugin ${QT_PLATFORM_PLUGINS})
    if(IS_DIRECTORY ${plugin})
        continue()
    endif(IS_DIRECTORY ${plugin})

    message(STATUS "Installing Qt platform plugin " ${plugin})
    file(COPY ${plugin} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/platforms)

    get_prerequisites(${plugin} PLUGIN_PREREQS ${EXCLUDE_SYSTEM_LIBS} 1 "" "")
    resolve_windows_prereqs(PLUGIN_PREREQS)

    message(STATUS "Installing prerequisites for " ${plugin})

    #Resolve symlinks
    set(resolvedFiles "")
    foreach(file ${PLUGIN_PREREQS})
        # Resolve symlink
        get_filename_component(resolvedFile "${file}" REALPATH)
        # Name of resolved symlink file
        get_filename_component(resolvedFileName "${resolvedFile}" NAME)
        # Name of original file
        get_filename_component(origFileName "${file}" NAME)
        # This nonsense is for copying the underlying library while giving it the name of its symlink
        # Why? Well, we want to package this with BlendToSMBStage2, but Blender plugins must be packaged as .ZIP files
        # .ZIP archives don't preserve symlinks, so this hacky nonsense avoids using them
        # Effectively, this should do nothing on platforms like Windows where symlinks aren't an issue
        string(CONCAT expectedFileName ${LIBPATH} "/" ${origFileName})
        string(CONCAT currentFileName ${LIBPATH} "/" ${resolvedFileName})
        file(COPY ${resolvedFile} DESTINATION ${LIBPATH})
        file(RENAME ${currentFileName} ${expectedFileName})
    endforeach(file)
endforeach()

