/// Sabre Includes
#include "sabre/member/property.hpp"
#include "sabre/object/guard.hpp"
#include "sabre/value/void.hpp"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Member::Property::getter(Runtime::Isolate *isolate, const Value::Any &self) const {
  $_UNUSED $_AUTO = Object::Guard(self); // lock the parent
  return m_getter ? m_getter(isolate, self) : Value::Missing();
}

Sabre::Value::Any
Sabre::Member::Property::setter(Runtime::Isolate *isolate, const Value::Any &self, const Value::Any &value) {
  $_UNUSED $_AUTO = Object::Guard(self); // lock the parent now
  return m_setter ? m_setter(isolate, self, value) : Value::Missing();
}