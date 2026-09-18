# --  PACKAGE SETUP  -- #

# Attempt requiring the desired packages
sabre_package_require(sabre)

# Prepare all the vendors to be required
sabre_vendors_require(CLI11)

# --  PACKAGE PROPERTIES  -- #

# Define this packages target
set(SABRE_TARGET_SHELL "shell" CACHE INTERNAL "")

# Get all the sources for the shell
sabre_sources_glob(SABRE_SOURCES_SHELL)

# --  PACKAGE DEFINITIONS  -- #

# Define the shell project now
project(${SABRE_TARGET_SHELL} VERSION ${SABRE_VERSION_SHORT} LANGUAGES CXX)

# Define the base executable details now
add_executable(${SABRE_TARGET_SHELL} ${SABRE_SOURCES_SHELL})

# Define all the libraries to be used
target_link_libraries(${SABRE_TARGET_SHELL} PUBLIC
    CLI11::CLI11
    mono::common
    sabre::crates
)

# Rename and move the resulting executable to "dist"
set_property(TARGET ${SABRE_TARGET_SHELL} PROPERTY OUTPUT_NAME ${SABRE_TARGET_SUPER})

# Only allow moving if we allow post-building
if (SABRE_OPTION_POSTBUILD)
    set_target_properties(${SABRE_TARGET_SHELL} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SABRE_DIRENT_BINARY})
endif ()

# Prepare a custom target for preparing the output directory with items
add_custom_target(sabre-output-dir ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${SABRE_DIRENT_BINARY})
add_custom_command(TARGET sabre-output-dir POST_BUILD COMMAND ${CMAKE_COMMAND}
    -DSABRE_OPTION_POSTBUILD=${SABRE_OPTION_POSTBUILD}
    -DSABRE_DIRENT_OUTPUT="${SABRE_DIRENT_OUTPUT}"
    -DSABRE_DIRENT_SCRIPT="${SABRE_DIRENT_SCRIPT}"
    -DSABRE_DIRENT_CRATES="${SABRE_DIRENT_SOURCE}/crates"
    -P "${SABRE_DIRENT_CMAKE}/include/post-build.cmake"
)
