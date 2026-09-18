/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  PRIVATE METHODS  //

Sabre::Member::View Sabre::Builtins::Wrapper<Sabre::Object::Instance>::m_attribute(
    const Object::Instance &self, const Value::Symbol &symbol
) {
  const auto &fields = self.fields(); // prepare base
  auto iter = fields.find(symbol), cend = fields.cend();
  return iter == cend ? nullptr : iter->second.get();
}
