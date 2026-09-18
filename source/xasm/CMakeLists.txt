# --  PACKAGE SETUP  -- #

# Prepare all the packages to be required
sabre_package_require(xtdlib)

# Prepare all the vendors to be required
sabre_vendors_require(asmjit)

# --  PACKAGE PROPERTIES  -- #

# Define the target to be used
set(SABRE_TARGET_XASM "xasm" CACHE INTERNAL "")

# Get all the availble sources for the project
sabre_sources_glob(SABRE_SOURCES_XASM)

# --  PACKAGE DEFINITION  -- #

# Define the project now
project(${SABRE_TARGET_XASM} VERSION ${SABRE_VERSION_SHORT} LANGUAGES CXX)

# Define the base library now
sabre_library_declare(${SABRE_TARGET_XASM} PCH SOURCES ${SABRE_SOURCES_XASM} LIBRARIES
    mono::common
    asmjit::asmjit
    sabre::xtdlib
)
