# --  PACKAGE SETUP  -- #

# Attempt requiring the desired packages
sabre_package_require(xtdlib)

# --  PACKAGE PROPERTIES  -- #

# Define the target to be used
set(SABRE_TARGET_XLSP "xlsp" CACHE INTERNAL "")

# Get all the sources for the runtime
sabre_sources_glob(SABRE_SOURCES_XLSP)

# --  PACKAGE DEFINITIONS  -- #

# Define the standard library project now
project(${SABRE_TARGET_XLSP} VERSION ${SABRE_VERSION_SHORT} LANGUAGES CXX)

# Define the runtime library now
sabre_library_declare(${SABRE_TARGET_XLSP} PCH SOURCES ${SABRE_SOURCES_XLSP} LIBRARIES
    mono::common
    sabre::xtdlib
)
