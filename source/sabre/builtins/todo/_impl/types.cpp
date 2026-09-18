/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Builtins::Custom::Todo>::m_typedefs(Type::World *globals) {
  // prepare the signature for a suitable "Todo" callback
  auto signature = TN::function(TN::never(), TN::optional(TN::string()));

  // and define the required types for "Todo"
  globals->types().declare(name(), TN::never());
  globals->values().declare(name(), signature);
}
