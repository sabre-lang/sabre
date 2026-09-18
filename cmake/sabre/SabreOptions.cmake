# --  OPTIONS  -- #

# Prepare some options to be used
option(SABRE_OPTION_CANARY "Enable canary builds" ON)
option(SABRE_OPTION_STRICT "Enable strict warnings" OFF)
option(SABRE_OPTION_TESTING "Enable building tests" OFF)
option(SABRE_OPTION_SANITIZE "Enable address sanitizer" OFF)
option(SABRE_OPTION_POSTBUILD "Enables post-build outputs" ON)

# Define the core compilation options to be used
set(SABRE_OPTION_CXXSTD "26" CACHE INTERNAL "The CXX standard library")
