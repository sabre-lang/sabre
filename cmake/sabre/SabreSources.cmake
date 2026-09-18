# --  PUBLIC FUNCTIONS  -- #

# Allow searching for incoming sources.
function(sabre_sources_glob _output)
    # parse the incoming arguments to be used
    cmake_parse_arguments(_ARGS "" "HINT" "INCLUDE" ${ARGN})

    mono_sources_glob(OUTPUT _sources BUILTINS HINT ${_ARGS_HINT} INCLUDE ${_ARGS_INCLUDE})
    set(${_output} ${_sources} PARENT_SCOPE) # and set the outgoing
endfunction()

# Allow searching for incoming tests.
function(sabre_sources_tests _output)
    # build the base sources to be used now
    mono_sources_glob(OUTPUT _sources BUILTINS)

    # attempt scanning for further tests now
    file(GLOB_RECURSE _tests ${SABRE_DIRENT_SOURCE}/*.test.cpp)
    list(APPEND _sources ${_tests}) # append the testing modules

    # and return the final result now
    set(${_output} ${_sources} PARENT_SCOPE)
endfunction()
