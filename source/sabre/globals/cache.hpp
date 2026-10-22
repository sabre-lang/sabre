#ifndef _SABRE_GLOBALS_IC_HPP
#define _SABRE_GLOBALS_IC_HPP

/// Sabre Includes
#include "sabre/forward/globals.hpp"
#include "sabre/value/symbol.hpp"

namespace Sabre::Globals {

/// @brief Globals Natives Cache.
class IC {
  //  TYPEDEFS  //

  /// @brief Allow the globals service access.
  friend class Service;

  //  PROPERTIES  //

  /// @brief Prepare a cache based on shape/symbols.
  $::Map::Base<Shape::Underlying, $::Map::Base<Value::Symbol, Value::Any>> m_cache = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an inline cache.
  constexpr IC() = default;

  //  PUBLIC METHODS  //

  /**
   * @brief Attempts getting a native value.
   * @param self                    Target self.
   * @param symbol                  Symbol of native.
   */
  Value::Any get(const Value::Any &self, const Value::Symbol &symbol) noexcept;

  /**
   * @brief Attempts setting a native value.
   * @param self                    Target self.
   * @param symbol                  Symbol of native.
   * @param value                   Value to inherit.
   */
  bool set(const Value::Any &self, const Value::Symbol &symbol, const Value::Any &value) noexcept;
};

} // namespace Sabre::Globals

#endif
