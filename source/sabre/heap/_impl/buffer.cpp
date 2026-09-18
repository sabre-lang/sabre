/// Sabre Includes
#include "sabre/heap/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/runtime/isolate.hpp"

//  CONSTRUCTORS  //

Sabre::Heap::Buffer::Buffer(Epoch epoch) : Buffer($::Global::get<Runtime::Container>(), epoch) {}
Sabre::Heap::Buffer::Buffer(Heap::Service *heap, Epoch epoch) : m_epoch(epoch), m_heap(heap) {}
Sabre::Heap::Buffer::Buffer(XI::Container *services, Epoch epoch) : Buffer(*services, epoch) {}

//  PRIVATE METHODS  //

void Sabre::Heap::Buffer::m_release($_UNUSED Address address, size_t size) {
  // validate some details about the current region
  $_ASSERT(m_region, "Cannot release buffer allocation from empty region");
  $_ASSERT(size <= m_region->used(), "Release size larger than region size");
  $_ASSERT(address == m_region->head() - size, "Mismatched heap-buffer release");

  // if the size was the same as the used, then pop the region
  if (m_region->release(size) == 0) m_region = m_heap->release(m_region);
}

Sabre::Heap::Address Sabre::Heap::Buffer::m_allocate(Runtime::Isolate *isolate, size_t size) {
  // check if the current region is valid
  bool exceeds = m_region == nullptr || !m_region->fits(size);

  // re-acquire a region now if necessary
  if (auto *thread = isolate->thread(); exceeds) thread->native([&] { m_acquire(); }), thread->checkpoint();

  // ensure we have some suitable results before continuing
  $_ASSERT(m_region, "Thread allocation region not acquired");
  $_ASSERT(m_region->fits(size), "Allocation does not fit region");
  $_ASSERT(m_region->epoch() == m_epoch, "Region generation mismatch");

  // return the allocation that occured now
  return m_region->allocate(size);
}

void Sabre::Heap::Buffer::m_acquire() {
  auto *parent = m_epoch == Epoch::STACK ? m_region : nullptr;
  m_region = m_heap->acquire(m_epoch, parent); // acquire now
}
