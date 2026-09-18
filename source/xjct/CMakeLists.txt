# --  PACKAGE SETUP  -- #

# Prepare all the vendors to be required
sabre_vendors_require(LIEF)

# Prepare all the packages to be required
sabre_package_require(xpc xtdlib)

# --  PACKAGE PROPERTIES  -- #

# Define the target to be used
set(SABRE_TARGET_XJCT "xjct" CACHE INTERNAL "")

# Get all the available sources for the project
sabre_sources_glob(SABRE_SOURCES_XJCT)

# --  PACKAGE DEFINITION  -- #

# Define the project now
project(${SABRE_TARGET_XJCT} VERSION ${SABRE_VERSION_SHORT} LANGUAGES CXX)

# Define the base library now
sabre_library_declare(${SABRE_TARGET_XJCT} PCH SOURCES ${SABRE_SOURCES_XJCT} LIBRARIES
    LIEF::LIEF
    mono::common
    sabre::xpc
    sabre::xtdlib
)
