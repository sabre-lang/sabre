/// Sabre Includes
#include "sabre/value/void.hpp"

/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

//  PRIVATE METHODS  //

Sabre::Value::Any Sabre::Builtins::Wrapper<Sabre::Value::Void>::m_globals(Isolate *) { return Value::Void(); }
