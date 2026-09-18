/// Sabre Includes
#include "sabre/dylib/addon.hpp"
#include "sabre/runtime/isolate.hpp"

//  PRIVATE METHODS  //

Sabre::Value::Any Sabre::Dylib::Addon::m_preload(Runtime::Isolate *isolate) {
  // ensure that we lock our instance when entering
  $_UNUSED $_AUTO = $::Lock::guard(m_mutex);

  // ignore loading if already loaded
  if (ready()) return m_exports;

  // construct the exports to be used
  auto exports = isolate->create<Exports>();

  // and load the library instance now
  if (m_loader) m_loader(isolate, exports);

  // return our resulting exports now
  return m_exports = exports;
}

void Sabre::Dylib::Addon::m_unload(Runtime::Isolate *) {
  // ensure that we lock our instance when entering
  $_UNUSED $_AUTO = $::Lock::guard(m_mutex);

  // only handle if exports have been constructed
  if (ready()) m_exports = Value::Void();
}