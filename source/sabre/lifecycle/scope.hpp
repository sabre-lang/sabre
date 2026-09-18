#ifndef _SABRE_LIFECYCLE_SCOPE_HPP
#define _SABRE_LIFECYCLE_SCOPE_HPP

/// Sabre Includes
#include "sabre/forward/lifecycle.hpp"
#include "sabre/forward/runtime.hpp"

namespace Sabre::Lifecycle {

/// @brief Scoped Lifecycle Handler.
class Scope : public XI::Transient {
  //  PROPERTIES  //

  /// @brief The lifecycle service.
  Service *m_lifecycle;

  /// @brief Bound isolate instance.
  Runtime::Isolate *m_isolate;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a lifecycle scoping.
   * @param services          Services container.
   * @param isolate           Runtime isolate.
   */
  explicit Scope(Runtime::Isolate *isolate = nullptr);
  explicit Scope(XI::Container *services, Runtime::Isolate *isolate = nullptr);

  /// @brief Handles destructing the scoping.
  ~Scope();
};

} // namespace Sabre::Lifecycle

#endif
