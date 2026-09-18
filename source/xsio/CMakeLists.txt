# --  PACKAGE SETUP  -- #

# Attempt requiring the desired packages
sabre_package_require(xtdlib)

# Prepare all the vendors to be required
sabre_vendors_require(boost)

# --  PACKAGE PROPERTIES  -- #

# Define the target to be used
set(SABRE_TARGET_XSIO "xsio" CACHE INTERNAL "")

# Get all the sources for the runtime
sabre_sources_glob(SABRE_SOURCES_XSIO)

# --  PACKAGE DEFINITIONS  -- #

# Define the standard library project now
project(${SABRE_TARGET_XSIO} VERSION ${SABRE_VERSION_SHORT} LANGUAGES CXX)

# Define the runtime library now
sabre_library_declare(${SABRE_TARGET_XSIO} PCH SOURCES ${SABRE_SOURCES_XSIO} LIBRARIES
    mono::common
    sabre::xtdlib
    Boost::context
)
