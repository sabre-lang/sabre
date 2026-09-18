#ifndef _SABRE_GLOBALS_ROOTS_HPP
#define _SABRE_GLOBALS_ROOTS_HPP

/// Sabre Includes
#include "sabre/forward/globals.hpp"
#include "sabre/value/common.hpp"

namespace Sabre::Globals {

/// @brief Global Roots Generator.
class Each {
  //  PROPERTIES  //

  /// @brief The underlying visitor callback.
  Visit m_visit = [](Value::Any &) {};

public:
  //  CONSTRUCTORS  //

  /// @brief Default construction.
  explicit Each() = default;

  /**
   * @brief Constructs an each callback.
   * @param visit               Visitor callback..
   */
  explicit Each(Visit &&visit) : m_visit(visit) {}

  //  OPERATOR METHODS  //

  /**
   * @brief Handles yielding values.
   * @param value                 Values to yield.
   */
  inline constexpr void operator()(Value::Any &value) noexcept { m_visit(value); }
  inline constexpr void operator()(const std::span<Value::Any> &values) noexcept {
    for (auto &value : values) m_visit(value); // iterate over the values now
  }
};

/// @brief Available Global Roots.
class Roots {
  //  PROPERTIES  //

  /// @brief Currently bound roots handlers.
  std::vector<$::Unique::Functor<void(Each &) const>> m_handlers = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted set of roots.
  explicit Roots() = default;

  //  PUBLIC METHODS  //

  /**
   * @brief Binds a roots callback.
   * @param handler                 Handler to bind.
   */
  inline constexpr void bind($::Unique::Functor<void(Each &) const> &&handler) {
    m_handlers.emplace_back(std::move(handler));
  }

  /**
   * @brief Handles safely iterating over the roots.
   * @param visit                   Visitor callback.
   */
  inline void each(Visit &&visit) noexcept {
    // prepare the each iterator now
    auto each = Each(std::move(visit));

    // prepare the underlying each handler
    for (const auto &handler : m_handlers) handler(each);
  }
};

} // namespace Sabre::Globals

#endif
