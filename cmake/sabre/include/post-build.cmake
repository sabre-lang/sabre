# --  MODULE INSTALLATION  -- #

# Ignore if in cross-compilation mode
if (NOT SABRE_OPTION_POSTBUILD)
    return()
endif ()

# Set a suitable shell extension now
if (WIN32)
    set(SABRE_EXTENSION_SCRIPT ".ps1")
else ()
    set(SABRE_EXTENSION_SCRIPT ".sh")
endif ()

# Remove the original files available in the output
file(REMOVE_RECURSE "${SABRE_DIRENT_OUTPUT}/crates")

# Get all the available ".sabre" and "_crate.jsonc" files
file(GLOB_RECURSE SABRE_SOURCES_SCRIPT "${SABRE_DIRENT_SCRIPT}/*${SABRE_EXTENSION_SCRIPT}")
file(GLOB_RECURSE SABRE_SOURCES_CRATES "${SABRE_DIRENT_CRATES}/*.sabre" "${SABRE_DIRENT_CRATES}/*.jsonc")

# Copy across the sources to the output directory necessary
foreach (_fp IN LISTS SABRE_SOURCES_SCRIPT)
    # ensure we ignore certain directories
    if (_fp MATCHES "compile")
        continue ()
    endif ()

    cmake_path(RELATIVE_PATH _fp BASE_DIRECTORY ${SABRE_DIRENT_SCRIPT})
    cmake_path(GET _fp PARENT_PATH _parent) # get the parent path to be used

    file(MAKE_DIRECTORY "${SABRE_DIRENT_OUTPUT}/scripts/${_parent}")
    file(COPY_FILE "${SABRE_DIRENT_SCRIPT}/${_fp}" "${SABRE_DIRENT_OUTPUT}/scripts/${_fp}" ONLY_IF_DIFFERENT)
endforeach ()

# Copy across the sources to the output directory necessary
foreach (_fp IN LISTS SABRE_SOURCES_CRATES)
    # ensure we ignore all testing files from the output
    if (_fp MATCHES ".test.sabre$")
        continue ()
    endif ()

    cmake_path(RELATIVE_PATH _fp BASE_DIRECTORY ${SABRE_DIRENT_CRATES})
    cmake_path(GET _fp PARENT_PATH _parent) # get the parent path to be used

    file(MAKE_DIRECTORY "${SABRE_DIRENT_OUTPUT}/crates/${_parent}")
    file(COPY_FILE "${SABRE_DIRENT_CRATES}/${_fp}" "${SABRE_DIRENT_OUTPUT}/crates/${_fp}" ONLY_IF_DIFFERENT)
endforeach ()
