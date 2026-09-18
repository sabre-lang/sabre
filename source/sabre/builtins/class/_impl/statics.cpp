/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  PRIVATE METHODS  //

Sabre::Value::Any Sabre::Builtins::Wrapper<Sabre::Object::Class>::m_globals(Isolate *, const Object::Class &self) {
  return self; // and return the resulting instance
}
