/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  PRIVATE METHODS  //

Sabre::Member::View
Sabre::Builtins::Wrapper<Sabre::Object::Class>::m_attribute(const Object::Class &self, const Value::Symbol &symbol) {
  const auto &statics = self.statics(); // prepare base
  auto iter = statics.find(symbol), cend = statics.cend();
  return iter == cend ? nullptr : iter->second.get();
}
