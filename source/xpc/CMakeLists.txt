# --  PACKAGE SETUP  -- #

# Attempt requiring the desired packages
sabre_package_require(xtdlib)

# --  PACKAGE PROPERTIES  -- #

# Define the target to be used
set(SABRE_TARGET_XPC "xpc" CACHE INTERNAL "")

# Get all the sources for the runtime
sabre_sources_glob(SABRE_SOURCES_XPC)

# --  PACKAGE DEFINITIONS  -- #

# Define the standard library project now
project(${SABRE_TARGET_XPC} VERSION ${SABRE_VERSION_SHORT} LANGUAGES CXX)

# Define the runtime library now
sabre_library_declare(${SABRE_TARGET_XPC} PCH SOURCES ${SABRE_SOURCES_XPC} LIBRARIES
    mono::common
    sabre::xtdlib
)
