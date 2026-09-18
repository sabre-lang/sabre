/// Crate Includes
#include "crates/yaml/source/addon.hpp"

//  PROPERTIES  //

/// @brief The underlying YAML addon installer.
SABRE_MM_DYLIB_ADDON(YAML, CRATE_XX_YAML_METHODS)

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(YAML, encode, isolate, ) { return isolate->todo(); }
SABRE_MM_DYLIB_METHOD(YAML, decode, isolate, ) { return isolate->todo(); }
