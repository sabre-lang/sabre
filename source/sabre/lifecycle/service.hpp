#ifndef _SABRE_LIFECYCLE_SERVICE_HPP
#define _SABRE_LIFECYCLE_SERVICE_HPP

/// Sabre Includes
#include "sabre/forward/runtime.hpp"
#include "sabre/lifecycle/scope.hpp"

namespace Sabre::Lifecycle {

/// @brief Disposable Callback.
using Disposable = $::Unique::Functor<void() const>;

/// @brief Constructs a lifecycle service.
class Service : public XI::Singleton {
  //  PROPERTIES  //

  /// @brief Internal disposable identifiers.
  size_t m_identifier = 0;

  /// @brief Services container.
  XI::Container *m_services;

  /// @brief All available disposables.
  $::Map::Base<size_t, Disposable> m_disposables = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a lifecycle service.
   * @param services                  Services container.
   */
  explicit Service();
  explicit Service(XI::Container *services);

  //  PUBLIC METHODS  //

  /**
   * @brief Scopes a lifecycle sequence.
   * @param isolate                   Runtime isolate.
   */
  Scope scope(Runtime::Isolate *isolate = nullptr);

  /**
   * @brief Handles preloading the lifecycle.
   * @param isolate                   Runtime isolate.
   */
  void preload(Runtime::Isolate *isolate = nullptr);

  /**
   * @brief Handles finalizing the lifecycle.
   * @param isolate                   Runtime isolate.
   */
  void unload(Runtime::Isolate *isolate = nullptr);

  /**
   * @brief Handles subscribing a disposable.
   * @param disposable                Disposable to subscribe.
   */
  inline constexpr Disposable subscribe(Disposable &&disposable) {
    auto identifier = m_identifier++; // prepare next identifier
    m_disposables.emplace(identifier, std::move(disposable));
    return [identifier, this] { m_disposables.erase(identifier); };
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Ensures some globals are preloaded.
   * @param isolate                   Runtime isolate.
   */
  void m_globals(Runtime::Isolate *isolate);
};

} // namespace Sabre::Lifecycle

#endif
