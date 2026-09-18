/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

/// Forward Declarations
$_FWD(Sabre::Builtins::Apply, static Value::Any iterator(Isolate *, const String::Any &))

//  PROPERTIES  //

/// @brief All available operators.
static auto s_operators = Sabre::Operator::Storage<Sabre::String::Any>({
    {Sabre::Operator::Kind::ITER, Sabre::Builtins::Apply::iterator},
});

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Apply::iterator(Isolate *isolate, const String::Any &self) {
  // prepare the iterator callback to be used
  Iterable::Callback<String::Any> callback = [](auto *, const String::Any &self, size_t index) -> Value::Any {
    return index < self.size() ? String::Small(self.rune(index)) : Value::Sentinel();
  };

  // and construct the resulting iterator to be used now
  return isolate->create<Iterable::Iterator>(self, std::move(callback));
}

//  PRIVATE METHODS  //

Sabre::Member::View Sabre::Builtins::Wrapper<Sabre::String::Any>::m_operator(const String::Any &, Operator::Kind kind) {
  return s_operators.lookup(kind);
}
