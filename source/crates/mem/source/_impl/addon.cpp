/// Crate Includes
#include "crates/mem/source/addon.hpp"

//  PROPERTIES  //

/// @brief The underlying memory addon installer.
SABRE_MM_DYLIB_ADDON(Memory, CRATE_XX_MEMORY_METHODS)

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(Memory, heap_usage, isolate, ) {
  Heap::Service *heap = *isolate->service();
  return Number::Tagged(heap->usage());
}

SABRE_MM_DYLIB_METHOD(Memory, heap_total, isolate, ) {
  Heap::Service *heap = *isolate->service();
  return Number::Tagged(heap->total());
}

SABRE_MM_DYLIB_METHOD(Memory, heap_avail, isolate, ) {
  Heap::Service *heap = *isolate->service();
  return Number::Tagged(heap->available());
}

SABRE_MM_DYLIB_METHOD(Memory, thread_usage, isolate, ) { return Number::Tagged(isolate->thread()->stack()->usage()); }
SABRE_MM_DYLIB_METHOD(Memory, thread_total, isolate, ) { return Number::Tagged(isolate->thread()->stack()->size()); }

SABRE_MM_DYLIB_METHOD(Memory, thread_avail, isolate, ) {
  return Number::Tagged(isolate->thread()->stack()->remaining());
}
