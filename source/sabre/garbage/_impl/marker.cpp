/// Sabre Includes
#include "sabre/garbage/marker.hpp"
#include "sabre/garbage/service.hpp"
#include "sabre/globals/roots.hpp"
#include "sabre/heap/service.hpp"
#include "sabre/runtime/container.hpp"

/// Value Includes
#include "sabre/value/_inline/value.ipp"

//  CONSTRUCTORS  //

Sabre::Garbage::Marker::Marker(Garbage::Service *garbage) : Marker($::Global::get<Runtime::Container>(), garbage) {}
Sabre::Garbage::Marker::Marker(XI::Container *services, Garbage::Service *garbage) :
    m_heap(*services), m_garbage(garbage) {}

//  PUBLIC METHODS  //

void Sabre::Garbage::Marker::mark(Mode mode, Globals::Roots *roots) {
  roots->each([&](Value::Any &value) { mark(mode, value); });
}

void Sabre::Garbage::Marker::mark(Mode mode, const Value::Any &value) {
  if (value.is<Object::Any>()) mark(mode, value.as<Object::Any>());
}

void Sabre::Garbage::Marker::mark(Mode mode, const Object::Any &object) {
  if (mode != Mode::MINOR || !object.pointer().is<Pointer::Kind::SEN>()) m_queue.push(object);
}

void Sabre::Garbage::Marker::trace(Mode mode) {
  // prepare a suitable mark handler
  auto yield = Globals::Each([&](Value::Any &value) { mark(mode, value); });

  // attempt trace across all the available object queue
  for (; m_queue.size(); m_queue.pop()) {
    // get the underlying object and its header
    auto *header = m_queue.front().header();

    // attempt setting the current object as being blackened and compact it
    if (m_blacken(header, yield)) m_compact(mode, header);
  }
}

//  PRIVATE METHODS  //

bool Sabre::Garbage::Marker::m_blacken(Object::Header *header, Globals::Each &yield) {
  // if the object was already reachable, then we ignore blackening
  if (!header->toggle<Object::Flag::REACHABLE>(true)) return false;

#define X(T, ...)                                                                       \
  case Shape::Lookup<T>(): Value::Inspect<T>::yield(header->encode<T>(), yield); break;
  switch (header->shape()) { SABRE_XX_VALUES_OBJECT(X) default : X(Object::Instance) }
#undef X

  // and declare that we successfully marked our items
  return true;
}

Sabre::Heap::Region *Sabre::Garbage::Marker::m_junior(Mode, size_t size) const noexcept {
  // prepare the predicate to be used
  auto predicate = [size](const Heap::Region *region) { return region->fits(size); };

  // otherwise find a current region that fits potentially
  if (auto *region = $::Ranges::Filter.first(m_garbage->m_juniors, predicate)) return region;

  // otherwise construct a new one to be used instead
  return *m_garbage->m_juniors.emplace(m_heap->acquire(Heap::Epoch::JUNIOR)).first;
}

Sabre::Heap::Region *Sabre::Garbage::Marker::m_senior(Mode mode, size_t size) const noexcept {
  // prepare the predicate to be used
  auto predicate = [size](const Heap::Region *region) { return region->fits(size); };

  // check if we can scan our current senior regions
  auto senior = mode == Mode::MINOR;

  // attempt a first get of a region here
  if (auto *region = senior ? $::Ranges::Filter.first(m_heap->storage()->senior, predicate) : nullptr) return region;

  // otherwise find a current region that fits potentially
  if (auto *region = $::Ranges::Filter.first(m_garbage->m_seniors, predicate)) return region;

  // otherwise construct a new one to be used instead
  return *m_garbage->m_seniors.emplace(m_heap->acquire(Heap::Epoch::SENIOR)).first;
}

Sabre::Heap::Region *
Sabre::Garbage::Marker::m_target(Mode mode, size_t size, size_t survivors, Heap::Epoch epoch) const noexcept {
  switch (epoch) {
  case Heap::Epoch::EDEN: return m_junior(mode, size);
  case Heap::Epoch::SENIOR: return m_senior(mode, size);

  // for junior regions we want to base on the survivor count
  case Heap::Epoch::JUNIOR: {
    auto senior = survivors >= m_heap->options()->garbage_survivors;
    return senior ? m_senior(mode, size) : m_junior(mode, size); // get
  }

  // should not normally reach here at all
  default: $_ABORT("Unexpected heap-region epoch");
  }
}

void Sabre::Garbage::Marker::m_compact(Mode mode, Object::Header *header) const noexcept {
  // get the underlying region being used
  auto epoch = header->region()->epoch();

  // cannot collect senior regions in minor-collection
  if (mode == Mode::MINOR && epoch == Heap::Epoch::SENIOR) return;

  // attempt getting the associated target region now
  auto *region = m_target(mode, header->size(), header->survivors(), epoch);
  $_ASSERT(region != nullptr, "Could not resolve a valid target region");

  // copy the header into the target region
  auto padding = sizeof(Object::Header);
  auto address = region->allocate(header->size());
  auto pointer = std::bit_cast<void *>(address);
  std::memcpy(pointer, std::bit_cast<void *>(header), padding);

#define X(T, ...)                                                                                  \
  case Shape::Lookup<T>(): Object::Allocator::move<T>(address + padding, header->encode()); break;
  switch (header->shape()) { SABRE_XX_VALUES_OBJECT(X) default : X(Object::Instance) }
#undef X

  // update all the flags for the newly constructed address now
  auto *target = reinterpret_cast<Object::Header *>(pointer);
  target->promote(region->epoch() == Heap::Epoch::SENIOR);

  // set the current forwarding target now as necessary
  header->forwarded(m_garbage->m_encode(target));

  // ensure reachability is valid now as well
  $_ASSERT(!target->reachable()), $_ASSERT(header->reachable());

  // prepare some validation just in case we messed up
  $_ASSERT(target->encode() == m_garbage->m_decode(header));
}
