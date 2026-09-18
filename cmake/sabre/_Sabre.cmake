# Ensure that CMake is configured correctly
cmake_minimum_required(VERSION 3.28.3)

# --  INCLUDES  -- #

# Ensure that we can only preload this module once
include_guard(GLOBAL)

# Prepare all the required CMake modules
include("${CMAKE_CURRENT_LIST_DIR}/../mono/_Mono.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/SabrePreload.cmake")
