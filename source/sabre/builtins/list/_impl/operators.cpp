/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

/// Forward Declarations
$_FWD(Sabre::Builtins::Apply, static Value::Any iterator(Isolate *, const Iterable::List &))

//  PROPERTIES  //

/// @brief All available operators.
static auto s_operators = Sabre::Operator::Storage<Sabre::Iterable::List>({
    {Sabre::Operator::Kind::ITER, Sabre::Builtins::Apply::iterator},
});

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Apply::iterator(Isolate *isolate, const Iterable::List &self) {
  // prepare the iterator callback to be used
  Iterable::Callback<Iterable::List> callback = [](auto *, const Iterable::List &self, size_t index) -> Value::Any {
    return index < self.size() ? self.get(index) : Value::Sentinel();
  };

  // and construct the resulting iterator to be used now
  return isolate->create<Iterable::Iterator>(self, std::move(callback));
}

//  PRIVATE METHODS  //

Sabre::Member::View
Sabre::Builtins::Wrapper<Sabre::Iterable::List>::m_operator(const Iterable::List &, Operator::Kind kind) {
  return s_operators.lookup(kind);
}
