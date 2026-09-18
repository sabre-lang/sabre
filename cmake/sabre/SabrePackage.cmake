# --  PUBLIC FUNCTIONS  -- #

# Handles declaring "sabre" packages
function(sabre_package_declare _package)
    set(_hint "${SABRE_DIRENT_SOURCE}/${_package}")
    mono_package_declare(${_package} HINT ${_hint})
endfunction()

# Handles requiring "sabre" packages
function(sabre_package_require)
    foreach (_package IN LISTS ARGN)
        if (TARGET ${_package})
            continue ()
        endif ()

        # can safely require the package now
        sabre_package_declare(${_package})
        mono_package_require(${_package})
    endforeach ()
endfunction()
