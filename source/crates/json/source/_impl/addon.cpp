/// Crate Includes
#include "crates/json/source/addon.hpp"

//  PROPERTIES  //

/// @brief The underlying JSON addon installer.
SABRE_MM_DYLIB_ADDON(JSON, CRATE_XX_JSON_METHODS)

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(JSON, encode, isolate, ) { return isolate->todo(); }
SABRE_MM_DYLIB_METHOD(JSON, decode, isolate, ) { return isolate->todo(); }
