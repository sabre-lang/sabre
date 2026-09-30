#ifndef _SABRE_ASYNC_ENTRY_HPP
#define _SABRE_ASYNC_ENTRY_HPP

/// Sabre Includes
#include "sabre/async/service.hpp"
#include "sabre/lifecycle/scope.hpp"
#include "sabre/runtime/executor.hpp"

namespace Sabre::Async {

/// @brief Describes a main-entry isolate.
class $_ABSTRACT Entry : public Runtime::Executor {
  //  PROPERTIES  //

  /// @brief Internal runtime scoping.
  $::Unique::Pointer<Lifecycle::Scope> m_lifecycle = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a main-isolate.
   * @param args              Executor arguments.
   */
  template <class... As> explicit Entry(As &&...args) : Executor(std::forward<As>(args)...) {
    service<Service>()->m_isolate = this;
  }

protected:
  //  PRIVATE METTHODS  //

  /// @brief Handles starting an entry scoping.
  inline constexpr void m_scope() {
    $_ASSERT(m_thread->worker(), "Worker not yet assigned");
    m_lifecycle = $::Unique::New<Lifecycle::Scope>(this);
  }
};

} // namespace Sabre::Async

#endif
