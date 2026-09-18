/// Sabre Includes
#include "sabre/string/common.hpp"
#include "sabre/runtime/isolate.hpp"

//  CONSTRUCTORS  //

Sabre::String::Any::Any(Runtime::Isolate *isolate, const Intern &intern) : Any() {
  if (intern.bytes() <= Limits::SMALL) m_pointer = Small(intern.view()).m_pointer;
  else m_pointer = isolate->create<String::Large>(intern).m_pointer;
}

Sabre::String::Any::Any(Runtime::Isolate *isolate, const $::String::View &value) : Any() {
  if (value.size() <= Limits::SMALL) m_pointer = Small(value).m_pointer;
  else m_pointer = isolate->create<String::Large>(value).m_pointer;
}
