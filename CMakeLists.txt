# Ensure that CMake is configured correctly
cmake_minimum_required(VERSION 3.28.3)

# --  INCLUDES  -- #

# Load the core project details to be used
include("${CMAKE_CURRENT_LIST_DIR}/cmake/sabre/_Sabre.cmake")

# --  CONDITIONS  -- #

# Stop early if user only wants to use "mono"
if (MONO_OPTION_ONLY)
    mono_message_status("Configured Module")
    return() # stop handling from here now
endif ()

# --  PROJECT  -- #

# Define the super-project instance now
project(${SABRE_TARGET_SUPER} VERSION ${SABRE_VERSION_SHORT} LANGUAGES CXX)

# --  INSTALL  -- #

# Check if running in the root-project
if (${SABRE_TOOLCHAIN_MAIN})
    sabre_message_status("Configuring Project '${SABRE_VERSION_LONG}'")
    add_subdirectory("${SABRE_DIRENT_SOURCE}/shell") # add entry-point
endif ()

# Enable all testing when desired
if (${SABRE_OPTION_TESTING})
    sabre_message_status("Enabled Testing '${SABRE_VERSION_LONG}'")
    add_subdirectory("${SABRE_DIRENT_SOURCE}/tests") # enable now
endif ()
