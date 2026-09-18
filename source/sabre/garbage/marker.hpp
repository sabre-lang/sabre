#ifndef _SABRE_GARBAGE_MARKER_HPP
#define _SABRE_GARBAGE_MARKER_HPP

/// Sabre Includes
#include "sabre/forward/globals.hpp"
#include "sabre/forward/heap.hpp"
#include "sabre/garbage/mode.hpp"
#include "sabre/heap/region.hpp"
#include "sabre/object/common.hpp"

namespace Sabre::Garbage {

/// @brief Handles marking objects.
class Marker : public XI::Transient {
  //  PROPERTIES  //

  /// @brief Underlying heap service.
  Heap::Service *m_heap;

  /// @brief The underlying garbage service.
  Garbage::Service *m_garbage;

  /// @brief Current queue of objects.
  std::queue<Object::Any> m_queue = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a garbage marker.
   * @param garbage               Garbage service.
   */
  explicit Marker(Garbage::Service *garbage);

  /**
   * @brief Constructs a garbage marker.
   * @param services              Services container.
   * @param garbage               Garbage service.
   */
  explicit Marker(XI::Container *services, Garbage::Service *garbage);

  //  PUBLIC METHODS  //

  /**
   * @brief Handles tracing marked queues.
   * @param mode              Collection mode.
   */
  void trace(Mode mode);

  /**
   * @brief Handles greying incoming values.
   * @param mode              Collection mode.
   * @param value             Value to mark.
   */
  void mark(Mode mode, const Value::Any &value);

  /**
   * @brief Handles greying incoming objects.
   * @param mode              Collection mode.
   * @param object            Object to mark.
   */
  void mark(Mode mode, const Object::Any &object);

  /**
   * @brief Handles greying global roots.
   * @param mode              Collection mode.
   * @param roots             Roots to be marked.
   */
  void mark(Mode mode, Globals::Roots *roots);

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles blackening an object.
   * @param header            Object header to blacken.
   * @param yield             Yield callback handler.
   */
  bool m_blacken(Object::Header *header, Globals::Each &yield);

  /**
   * @brief Handles reallocating objects to other regions.
   * @param mode              Collection mode.
   * @param header            Object header.
   */
  void m_compact(Mode mode, Object::Header *header) const noexcept;

  /**
   * @brief Gets the target region for a header.
   * @param mode              Collection mode.
   * @param size              Allocation size.
   */
  Heap::Region *m_junior(Mode mode, size_t size) const noexcept;
  Heap::Region *m_senior(Mode mode, size_t size) const noexcept;

  /**
   * @brief Gets the target region for a header.
   * @param mode              Collection mode.
   * @param size              Allocation size.
   * @param survivors         Survivor count.
   * @param epoch             Heap region epoch.
   */
  Heap::Region *m_target(Mode mode, size_t size, size_t survivors, Heap::Epoch epoch) const noexcept;
};

} // namespace Sabre::Garbage

#endif
