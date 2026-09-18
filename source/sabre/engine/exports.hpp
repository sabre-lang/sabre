#ifndef _SABRE_ENGINE_EXPORTS_HPP
#define _SABRE_ENGINE_EXPORTS_HPP

/// Sabre Includes
#include "sabre/forward/engine.hpp"
#include "sabre/value/void.hpp"

namespace Sabre::Engine {

/// @brief Engine Exports Container.
class Exports {
  //  PROPERTIES  //

  /// @brief Currently exported values.
  std::vector<Value::Any> m_scopes = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty set of exports.
  explicit Exports() = default;

  //  PUBLIC METHODS  //

  /// @brief Gets the available scopes.
  inline constexpr std::vector<Value::Any> &scopes() noexcept { return m_scopes; }
  inline constexpr const std::vector<Value::Any> &scopes() const noexcept { return m_scopes; }

  /// @brief Gets the current exports level.
  inline constexpr Value::Any current() const noexcept { return m_scopes.empty() ? Value::Void() : m_scopes.back(); }

  /// @brief Closes a set of exports.
  inline constexpr Value::Any close() { return m_close(); }

  /**
   * @brief Begins a new set of exports.
   * @param isolate                   Runtime isolate.
   */
  inline constexpr Value::Any open(Isolate *isolate) { return m_open(isolate); }

private:
  //  PRIVATE METHODS  //

  /// @brief Closes a set of exports.
  Value::Any m_close();

  /**
   * @brief Begins a new set of exports.
   * @param isolate                   Runtime isolate.
   */
  Value::Any m_open(Isolate *isolate);
};

} // namespace Sabre::Engine

#endif
