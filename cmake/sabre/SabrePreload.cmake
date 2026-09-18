# --  CONDITIONS  -- #

# Stop early if user only wants to use "mono"
if (MONO_OPTION_ONLY)
    return()
endif ()

# --  INCLUDES  -- #

# Include all the "sabre" base modules
include("${CMAKE_CURRENT_LIST_DIR}/SabreOptions.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/SabreMetadata.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/SabreMessage.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/SabreLibrary.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/SabrePackage.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/SabreSources.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/SabreVendors.cmake")

# --  FEATURES  -- #

# Ensure some features are enabled
mono_sccache_setup()
mono_feature_ninja()

# Ensure all the "mono" libraries are enabled
mono_library_warnings(${SABRE_OPTION_STRICT})
mono_library_sanitize(${SABRE_OPTION_SANITIZE})
mono_library_common(${SABRE_OPTION_CXXSTD})
