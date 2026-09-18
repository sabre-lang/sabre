/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Value::Void>::m_typedefs(Type::World *globals) {
  globals->types().declare(name(), TN::none());
  globals->values().declare(name(), TN::none());
}
