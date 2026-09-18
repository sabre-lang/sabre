/// Crate Includes
#include "crates/toml/source/addon.hpp"

//  PROPERTIES  //

/// @brief The underlying TOML addon installer.
SABRE_MM_DYLIB_ADDON(TOML, CRATE_XX_TOML_METHODS)

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(TOML, encode, isolate, ) { return isolate->todo(); }
SABRE_MM_DYLIB_METHOD(TOML, decode, isolate, ) { return isolate->todo(); }
