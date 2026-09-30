/// Sabre Includes
#include "sabre/object/allocator.hpp"
#include "sabre/heap/service.hpp"
#include "sabre/runtime/isolate.hpp"

/// Value Includes
#include "sabre/value/_inline/value.ipp"

//  PRIVATE METHODS  //

Sabre::Heap::Address Sabre::Object::Allocator::m_new(Runtime::Isolate *isolate, size_t size, Shape::Underlying shape) {
  auto *heap = isolate->service<Heap::Service>();
  auto *buffer = heap->allocator(isolate->thread());

  // prepare some constexpr values to be used
  constexpr auto s_padding = sizeof(Object::Header);

  // ensure we fix the incoming size to be used
  size = Heap::Align(size + s_padding);

  // attempt actually allocating now
  auto address = buffer->allocate(isolate, size);
  auto *header = std::bit_cast<Header *>(address);

  // and initialize the header instance as well
  return new (header) Header(shape, size), address + s_padding;
}

bool Sabre::Object::Allocator::m_destruct(const Value::Any &value) {
  if (!value.is<Object::Any>()) return false;
  return m_destruct(value.as<Object::Any>().header()), true;
}

void Sabre::Object::Allocator::m_destruct(const Object::Header *header) {
#define X(T, ...)                                                                  \
  case Shape::Lookup<T>(): Object::Allocator::destroy<T>(header->encode()); break;
  switch (header->shape()) { SABRE_XX_VALUES_OBJECT(X) default : X(Object::Instance) }
#undef X
}
