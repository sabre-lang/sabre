#ifndef _SABRE_HEAP_SERVICE_HPP
#define _SABRE_HEAP_SERVICE_HPP

/// Sabre Includes
#include "sabre/forward/garbage.hpp"
#include "sabre/heap/buffer.hpp"
#include "sabre/heap/options.hpp"
#include "sabre/heap/storage.hpp"

namespace Sabre::Heap {

/// @brief Heap Service.
class Service : public XI::Singleton, public XI::Immediate {
  //  TYPEDEFS  //

  /// @brief Allow GC internal access.
  friend class Garbage::Service;

  //  PROPERTIES  //

  /// @brief Heap access mutex.
  mutable $::Mutex::Auto m_mutex;

  /// @brief Heap address pointer.
  void *m_data = nullptr;

  /// @brief Encapsulated heap options.
  const Options *m_options;

  /// @brief Services container.
  XI::Container *m_services;

  /// @brief Bound stack buffers.
  $::Map::Base<const Runtime::Isolate *, Buffer> m_stacks = {};

  /// @brief Bound allocation buffers.
  $::Map::Base<const Async::Processor *, Buffer> m_buffers = {};

  /// @brief Internal heap-regions available.
  $::Unique::Pointer<Storage> m_storage = $::Unique::New<Storage>();

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a new heap-service.
   * @param services                  Services container.
   */
  explicit Service();
  explicit Service(XI::Container *services);

  /// @brief Handles destroying heaped memory.
  ~Service();

  //  PUBLIC METHODS  //

  /// @brief Gets the total available heap-space.
  inline constexpr size_t total() const noexcept { return Limits::ADDRESS_SPACE; }

  /// @brief Gets the available memory remaining.
  inline constexpr size_t available() const noexcept { return total() - usage(); }

  /// @brief Gets the current heap options available.
  inline constexpr const Options *options() const noexcept { return m_options; }

  /// @brief Gets the underlying heap-address.
  inline constexpr Address address() const noexcept { return std::bit_cast<Address>(m_data); }

  /// @brief Gets a view of the current garbage regions.
  inline constexpr const Storage *storage() const noexcept { return m_storage.get(); }

  /// @brief Grows the underlying heap.
  void grow();

  /// @brief Shrinks the underlying heap.
  void shrink();

  /// @brief Gets the current usage.
  size_t usage() const noexcept;

  /**
   * @brief Allows releasing a region (expected to be empty).
   * @param region                    REgion to remove.
   */
  Region *release(Region *region);

  /**
   * @brief Attempts aquiring a generation.
   * @param epoch                     Epoch to acquire.
   * @param parent                    Linked region.
   */
  Region *acquire(Epoch epoch, Region *parent = nullptr);

  /**
   * @brief Constructs a buffer instance.
   * @param epoch                     Epoch to bind.
   */
  inline constexpr Buffer buffer(Epoch epoch = Epoch::EDEN) { return Buffer(this, epoch); }

  /**
   * @brief Gets an isolates stack buffer.
   * @param isolate                   Runtime isolate.
   */
  inline constexpr Buffer *stack(const Runtime::Isolate *isolate) {
    if (m_stacks.contains(isolate)) return &m_stacks.at(isolate);
    return &m_stacks.emplace(isolate, buffer(Epoch::STACK)).first->second;
  }

  /**
   * @brief Gets an isolates current allocator.
   * @param thread                    Virtual thread.
   */
  inline constexpr Buffer *allocator(Async::Thread *thread) { return allocator(thread->worker()->processor()); }

  /**
   * @brief Gets a processors allocator.
   * @param processor                 Processor reference.
   */
  inline constexpr Buffer *allocator(const Async::Processor *processor) {
    if (m_buffers.contains(processor)) return &m_buffers.at(processor);
    return &m_buffers.emplace(processor, buffer(Epoch::EDEN)).first->second;
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Unmaps free regions.
  void m_unmap();

  /// @brief Maps any free regions.
  Region *m_remap();

  /// @brief Pops any free regions.
  Region *m_pop();
};

} // namespace Sabre::Heap

#endif
