# --  PROPERTIES  -- #

# Resolve the root of the project
set(SABRE_DIRENT_CMAKE "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")
cmake_path(SET SABRE_DIRENT_ROOT "${SABRE_DIRENT_CMAKE}/../.." NORMALIZE)
string(REGEX REPLACE "${MONO_PATH_SEP}$" "" SABRE_DIRENT_ROOT ${SABRE_DIRENT_ROOT})

# Define all the additional directories to be used
set(SABRE_DIRENT_OUTPUT "${SABRE_DIRENT_ROOT}/dist" CACHE INTERNAL "")
set(SABRE_DIRENT_BINARY "${SABRE_DIRENT_OUTPUT}/bin" CACHE INTERNAL "")
set(SABRE_DIRENT_SOURCE "${SABRE_DIRENT_ROOT}/source" CACHE INTERNAL "")
set(SABRE_DIRENT_SCRIPT "${SABRE_DIRENT_ROOT}/scripts" CACHE INTERNAL "")
set(SABRE_DIRENT_CONFIG "${SABRE_DIRENT_ROOT}/configs" CACHE INTERNAL "")

# Resolve the versioning to be used
if (NOT SABRE_OPTION_CANARY)
    # We have been requested to define a non-canary build
    mono_version_read(SABRE_VERSION "${SABRE_DIRENT_CONFIG}/version.txt" COMMIT SUFFIX "stable")
else ()
    # We define canary builds by default (since this declares custom configurations)
    mono_version_read(SABRE_VERSION "${SABRE_DIRENT_CONFIG}/version.txt" COMMIT SUFFIX "canary")
endif ()

# Prepare all the baseline properties
set(SABRE_TOOLCHAIN_TITLE "Sabre" CACHE INTERNAL "")
set(SABRE_TOOLCHAIN_STRING "${SABRE_TOOLCHAIN_TITLE} ${SABRE_VERSION_LONG}" CACHE INTERNAL "")
set(SABRE_TOOLCHAIN_TARNAME "${SABRE_TOOLCHAIN_TITLE}_${SABRE_VERSION_LONG}" CACHE INTERNAL "")

# Determine if in the main-project or as a dependency
if (CMAKE_SOURCE_DIR STREQUAL SABRE_DIRENT_ROOT)
    # We set the toolchain as being the root
    set(SABRE_TOOLCHAIN_MAIN ON CACHE INTERNAL "")

    # And we should also enable some additional features
    mono_feature_responses()
    mono_feature_visibility()
    mono_feature_deprecation(OFF)
else ()
    set(SABRE_TOOLCHAIN_MAIN OFF CACHE INTERNAL "")
endif ()

# Define the primary targets to be available
set(SABRE_TARGET_SUPER "sabre" CACHE INTERNAL "")
