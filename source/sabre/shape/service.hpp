#ifndef _SABRE_SHAPE_SERVICE_HPP
#define _SABRE_SHAPE_SERVICE_HPP

/// Sabre Includes
#include "sabre/forward/syntax.hpp"
#include "sabre/shape/lookup.hpp"

namespace Sabre::Shape {

/// @brief Shape Service.
class Service : public XI::Singleton {
  //  PROPERTIES  //

  /// @brief The mutex for shape access.
  mutable $::Mutex::Auto m_mutex;

  /// @brief Currently bound shapes.
  $::Map::Base<const Syntax::Node *, Underlying> m_bound = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs the shape service.
  constexpr Service() = default;

  //  PUBLIC METHODS  //

  /**
   * @brief Checks if a shape exists.
   * @param variable                  Variable to check.
   */
  inline constexpr bool contains(const Syntax::Node *variable) const noexcept { return m_bound.contains(variable); }

  /**
   * @brief Handle assigning shapes.
   * @param variable                  Variable to resolve.
   */
  inline constexpr Underlying resolve(const Syntax::Node *variable) const noexcept {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex);
    auto iterator = m_bound.find(variable);
    return iterator == m_bound.end() ? Invalid() : iterator->second;
  }

  /**
   * @brief Handle assigning shapes.
   * @param variable                  Variable to assign.
   */
  inline constexpr Underlying assign(const Syntax::Node *variable) noexcept {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex); // ensure any assignments are locked
    return m_bound.try_emplace(variable, Limits::THRESHOLD + m_bound.size() + 1).first->second;
  }
};

} // namespace Sabre::Shape

#endif
