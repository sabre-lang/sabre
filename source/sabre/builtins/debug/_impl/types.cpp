/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Declarations
$_FWD(Sabre::Builtins, using TN = Type::New)

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Builtins::Custom::Debug>::m_typedefs(Type::World *globals) {
  // prepare the fields to be used
  auto fields = $::Map::Record<Type::Entity>();

  // bind all the underlying fields to be used now
#define SABRE_XX_FIELDS_DEFINE(N, ...) fields.emplace(#N, TN::variadic(TN::none()));
#include "sabre/builtins/debug/_defines/fields.def"
#undef SABRE_XX_FIELDS_DEFINE

  // expose the "Debug" protocol only as a value
  globals->values().declare(name(), TN::interface(name(), fields));
}
