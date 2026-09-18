/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  PRIVATE METHODS  //

Sabre::Value::Any Sabre::Builtins::Wrapper<Sabre::Object::Instance>::m_globals(Isolate *, const Object::Class &self) {
  return self;
}
