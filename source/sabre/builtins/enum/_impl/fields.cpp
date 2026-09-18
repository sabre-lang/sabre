/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  PRIVATE METHODS  //

Sabre::Member::View
Sabre::Builtins::Wrapper<Sabre::Object::Enum>::m_attribute(const Object::Enum &self, const Value::Symbol &symbol) {
  auto *variant = self.resolve(symbol); // find
  return variant ? variant->value.get() : nullptr;
}
