/// Sabre Includes
#include "sabre/member/reference.hpp"
#include "sabre/object/guard.hpp"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Member::Reference::getter(Runtime::Isolate *, const Value::Any &self) const {
  $_UNUSED $_AUTO = Object::Guard(self);
  return m_reference; // return result
}

Sabre::Value::Any
Sabre::Member::Reference::setter(Runtime::Isolate *, const Value::Any &self, const Value::Any &value) {
  $_UNUSED $_AUTO = Object::Guard(self);
  if (!readonly()) return m_reference = value;
  return Value::Void(Value::Feedback::FIELD_IMMUTABLE);
}