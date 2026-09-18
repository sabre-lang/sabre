# --  PACKAGE SETUP  -- #

# Attempt requiring the desired packages
sabre_package_require(xasm xinv xlsp xpc xjct xsio xtdlib xtest)

# Prepare all the vendors to be required
sabre_vendors_require(boost)

# --  PACKAGE PROPERTIES  -- #

# Define the target to be used
set(SABRE_TARGET_CRATES "crates" CACHE INTERNAL "")

# Get all the sources for the runtime
sabre_sources_glob(SABRE_SOURCES_SUPER INCLUDE "_parse" "_format" "_check" "_lower" "_rules" "_relint")
sabre_sources_glob(SABRE_SOURCES_CRATES HINT "${SABRE_DIRENT_SOURCE}/crates")

# Merge all the source files together as necessary
list(APPEND SABRE_SOURCES_SUPER ${SABRE_SOURCES_CRATES})

# --  PACKAGE DEFINITIONS  -- #

# Define the runtime library now
sabre_library_declare(${SABRE_TARGET_SUPER} AUTOGEN SOURCES ${SABRE_SOURCES_SUPER} LIBRARIES
    mono::common
    sabre::xasm
    sabre::xinv
    sabre::xpc
    sabre::xlsp
    sabre::xjct
    sabre::xsio
    sabre::xtest
    sabre::xtdlib
    Boost::locale
)

# Ensure we allow injecting vendors as well
sabre_vendors_inject(${SABRE_TARGET_SUPER})

# Define some common definitions now
target_compile_definitions(${SABRE_TARGET_SUPER} PUBLIC
    -DSABRE_MM_TITLE="${SABRE_TOOLCHAIN_TITLE}"
    -DSABRE_MM_IDENTIFIER="${SABRE_TARGET_SUPER}"
    -DSABRE_MM_VERSION="${SABRE_VERSION_LONG}"
    -DSABRE_MM_COMMIT="${SABRE_VERSION_COMMIT}"
    -DSABRE_MM_CANARY=$<BOOL:${SABRE_OPTION_CANARY}>
    -DSABRE_MM_SANITIZE=$<BOOL:${SABRE_OPTION_SANITIZE}>
)

# Define a custom library for binding the crates to the runtime
add_library(${SABRE_TARGET_CRATES} INTERFACE)
add_library(${SABRE_TARGET_SUPER}::${SABRE_TARGET_CRATES} ALIAS ${SABRE_TARGET_CRATES})

# Define the additional "sabre::crates" details
target_link_libraries(${SABRE_TARGET_CRATES} INTERFACE $<LINK_LIBRARY:WHOLE_ARCHIVE,sabre::sabre>)
