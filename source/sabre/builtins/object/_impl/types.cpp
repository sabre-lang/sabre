/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)

$::Shared::Pointer<Sabre::Type::Prototype> Sabre::Builtins::Wrapper<Sabre::Object::Instance>::typeclass() {
  return m_typeclass([](const auto &) {});
}

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Object::Instance>::m_typedefs(Type::World *globals) {
  // get the underlying prototype instance
  auto prototype = typeclass();

  // prepare the baseline typing to be used
  globals->values().declare(name(), prototype);
  globals->types().declare(name(), TN::object());
}
