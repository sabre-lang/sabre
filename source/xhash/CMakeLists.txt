# --  PACKAGE SETUP  -- #

# Prepare all the vendors to be required
sabre_vendors_require(ankerl muuid)

# --  PACKAGE PROPERTIES  -- #

# Define the target to be used
set(SABRE_TARGET_XHASH "xhash" CACHE INTERNAL "")

# Get all the sources for the runtime
sabre_sources_glob(SABRE_SOURCES_XHASH)

# --  PACKAGE DEFINITIONS  -- #

# Define the standard library project now
project(${SABRE_TARGET_XHASH} VERSION ${SABRE_VERSION_SHORT} LANGUAGES CXX)

# Define the runtime library now
sabre_library_declare(${SABRE_TARGET_XHASH} PCH SOURCES ${SABRE_SOURCES_XHASH} LIBRARIES
    mono::common
    modern-uuid-static
    unordered_dense::unordered_dense
)
