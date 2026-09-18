# --  PUBLIC FUNCTIONS  -- #

# Allows declaring libraries
function(sabre_library_declare _target)
    # show that we have declared this library instance
    sabre_message_status("Configuring Library '${_target}'")

    # parse the incoming arguments now
    cmake_parse_arguments(_ARGS "PCH;AUTOGEN" "" "SOURCES;LIBRARIES" ${ARGN})

    # prepare the available sources
    if (NOT DEFINED _ARGS_SOURCES)
        set(_ARGS_SOURCES)
    endif ()

    # handle based on the sources available
    if (_ARGS_SOURCES)
        set(_type PRIVATE) # declare as private now
        set(_include PUBLIC) # allow includes to be exposed
        add_library(${_target} STATIC) # static library
        target_sources(${_target} ${_type} ${_ARGS_SOURCES})
    else ()
        set(_type INTERFACE) # hidden
        set(_include ${_type}) # hidden
        add_library(${_target} ${_type})
    endif ()

    # and add the exposed library instance
    add_library(${SABRE_TARGET_SUPER}::${_target} ALIAS ${_target})
    target_include_directories(${_target} ${_include} ${SABRE_DIRENT_SOURCE})

    # handle whether or not there are auto-generation files
    if (_ARGS_AUTOGEN)
        set(_autogen "${CMAKE_CURRENT_BINARY_DIR}/autogen/include")
        target_include_directories(${_target} ${_include} ${_autogen})
    endif ()

    # handling incoming pre-compilation libraries as well
    if (_ARGS_PCH)
        target_precompile_headers(${_target} ${_include} "${_target}.hpp")
    endif ()

    # handle adding link-libraries
    if (DEFINED _ARGS_LIBRARIES)
        target_link_libraries(${_target} ${_include} ${_ARGS_LIBRARIES})
    endif ()
endfunction()
