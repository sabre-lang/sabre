/// Sabre Includes
#include "sabre/function/environ.hpp"
#include "sabre/runtime/isolate.hpp"

//  CONSTRUCTORS  //

Sabre::Function::Environ::Environ(Runtime::Isolate *isolate, size_t size, const Environ &parent) : Environ() {
  if (size) m_value = isolate->create<Iterable::List>(Offset::VALUES + size);
  if (parent.valid()) m_set(Offset::PARENT, parent.m_value); // bind parent
}
