/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Builtins::Custom::Utility>::m_typedefs(Type::World *globals) {
  // prepare the baseline intrinsic types to be used
  globals->types().declare("Any", TN::any());
  globals->types().declare("Never", TN::never());

  // define the generic "Maybe" typing using a required generic
  auto M = Sabre::Builtins::TN::constraint("T", TN::any());
  globals->types().declare("Maybe", TN::generic(TN::maybe(M), M));

  // define the generic "Record" typing using a suitable generic
  auto R = Sabre::Builtins::TN::constraint("T", TN::any());
  globals->types().declare("Record", TN::generic(TN::record(R), R));
}
