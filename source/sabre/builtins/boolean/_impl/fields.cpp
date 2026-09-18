/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  PRIVATE METHODS  //

Sabre::Member::View
Sabre::Builtins::Wrapper<Sabre::Value::Boolean>::m_attribute(const Value::Boolean &, const Value::Symbol &) {
  return nullptr;
}
