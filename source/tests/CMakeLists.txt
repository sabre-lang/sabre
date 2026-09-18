# --  PACKAGE SETUP  -- #

# Attempt requiring the desired packages
sabre_package_require(sabre xtest)

# --  PACKAGE PROPERTIES  -- #

# Define this packages target
set(SABRE_TARGET_TESTS "tests" CACHE INTERNAL "")

# Get all the available sources for the testing executable
sabre_sources_tests(SABRE_SOURCES_TESTS)

# --  PACKAGE DEFINITIONS  -- #

# Define the tests project now
project(${SABRE_TARGET_TESTS} VERSION ${SABRE_VERSION_SHORT} LANGUAGES CXX)

# Define the base executable now
add_executable(${SABRE_TARGET_TESTS} ${SABRE_SOURCES_TESTS})

# Define all the libraries to be used now
target_link_libraries(${SABRE_TARGET_TESTS} PUBLIC
    mono::common
    sabre::xtest
    sabre::crates
)
