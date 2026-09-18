#ifndef _SABRE_VALUE_INSPECT_HPP
#define _SABRE_VALUE_INSPECT_HPP

/// Sabre Includes
#include "sabre/builtins/inspect.hpp"
#include "sabre/shape/lookup.hpp"

namespace Sabre::Value {

/// @brief Allows Value Inspection.
template <std::derived_from<Any> T> class Inspect : protected T {
  //  PROPERTIES  //

  /// @brief Checks if there is a suitable "m_yield" method attached.
  static inline constexpr bool m_roots = requires(const T &self, Globals::Each &yield) {
    { T::m_yield(self, yield) } -> std::same_as<void>;
  };

public:
  //  PUBLIC METHODS  //

  /// @brief Gets the associated shape value.
  static inline constexpr Shape::Underlying shape() { return Shape::Lookup<T>(); }

  /// @brief Gets the associated value name.
  static inline constexpr $::String::View name() { return Builtins::Inspect<T>::name(); }

  /**
   * @brief Handles validation of values.
   * @param pointer                 Tagged pointer.
   */
  static inline constexpr bool is(const Pointer::Tagged &pointer) { return T::m_is(pointer); }

  /**
   * @brief Gets the associated pointers truthiness.
   * @param value                 Value to resolve.
   */
  static inline constexpr bool truthiness(const T &value) { return T::m_truthiness(value); }

  /**
   * @brief Handles iterating over object roots.
   * @param value                 Value to yield.
   * @param yield                 Yield callback.
   */
  static inline constexpr void yield(const T &value, Globals::Each &yield) {
    if constexpr (m_roots) T::m_yield(value, yield); // call if available
  }
};

} // namespace Sabre::Value

#endif
