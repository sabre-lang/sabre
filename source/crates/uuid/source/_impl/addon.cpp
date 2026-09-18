/// Crate Includes
#include "crates/uuid/source/addon.hpp"

//  PROPERTIES  //

/// @brief The underlying UUID addon installer.
SABRE_MM_DYLIB_ADDON(UUID, CRATE_XX_UUID_METHODS)

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(UUID, V1, isolate, ) { return m_generate(isolate, XH::UUID::V1); }
SABRE_MM_DYLIB_METHOD(UUID, V4, isolate, ) { return m_generate(isolate, XH::UUID::V4); }
SABRE_MM_DYLIB_METHOD(UUID, V6, isolate, ) { return m_generate(isolate, XH::UUID::V6); }
SABRE_MM_DYLIB_METHOD(UUID, V7, isolate, ) { return m_generate(isolate, XH::UUID::V7); }

SABRE_MM_DYLIB_METHOD(UUID, V3, isolate, args) { return m_namespace(isolate, args, XH::UUID::V3); }
SABRE_MM_DYLIB_METHOD(UUID, V5, isolate, args) { return m_namespace(isolate, args, XH::UUID::V5); }
