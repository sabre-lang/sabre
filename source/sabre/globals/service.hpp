#ifndef _SABRE_GLOBALS_SERVICE_HPP
#define _SABRE_GLOBALS_SERVICE_HPP

/// Sabre Includes
#include "sabre/forward/type.hpp"
#include "sabre/function/callback.hpp"
#include "sabre/globals/roots.hpp"
#include "sabre/object/class.hpp"
#include "sabre/value/symbol.hpp"

namespace Sabre::Globals {

/// @brief Builtin Global Values.
class Service : public XI::Singleton, public XI::Immediate {
  //  PROPERTIES  //

  /// @brief Ensure values can only be accessed one-at-a-time.
  mutable $::Mutex::Shared m_mutex;

  /// @brief Available global types world.
  $::Unique::Pointer<Type::World> m_types;

  /// @brief Currently instantiated values (eg: runtime-values).
  $::Map::Base<Value::Symbol, Value::Any> m_values = {};

  /// @brief Available internal values (eg: native-functions).
  $::Map::Base<Value::Symbol, Value::Any> m_internal = {};

  /// @brief Preloaded factories for use.
  $::Map::Base<Value::Symbol, Factory> m_factories = {};

  /// @brief Explicitly available global roots.
  $::Unique::Pointer<Roots> m_roots = $::Unique::New<Roots>();

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a set of global values.
  explicit Service();

  //  PUBLIC METHODS  //

  /// @brief Gets all the available runtime roots.
  inline constexpr Roots *roots() const noexcept { return m_roots.get(); }

  /// @brief Gets the available global types.
  inline constexpr Type::World *types() const noexcept { return m_types.get(); }

  /**
   * @brief Denotes if a symbol exists.
   * @param symbol                Symbol to query.
   */
  inline constexpr bool has(const Value::Symbol &symbol) const noexcept {
    return m_values.contains(symbol) || m_factories.contains(symbol);
  }

  /**
   * @brief Explicitly casts a global to a type.
   * @param thread                Runtime Isolate.
   * @param symbol                Global symbol.
   */
  template <std::derived_from<Value::Any> T>
  inline constexpr T get(Runtime::Isolate *isolate, const Value::Symbol &symbol) {
    return get(isolate, symbol).as<T>();
  }

  /**
   * @brief Attempts getting a global prototype.
   * @param isolate               Runtime Isolate.
   */
  template <std::derived_from<Value::Any> T> inline constexpr Object::Class prototype(Runtime::Isolate *isolate) {
    return get<Object::Class>(isolate, Builtins::Inspect<T>::name());
  }

  /**
   * @brief Allow getting cached persitent globals.
   * @param isolate               Runtime Isolate.
   * @param symbol                Global symbol.
   * @param factory               Factory to use.
   */
  template <std::derived_from<Value::Any> T>
  inline constexpr T internal(Runtime::Isolate *isolate, const Value::Symbol &symbol, Factory &&factory) {
    return internal(isolate, symbol, std::move(factory)).as<T>();
  }

  /**
   * @brief Allow getting cached persitent globals.
   * @param isolate               Runtime Isolate.
   * @param symbol                Global symbol.
   * @param factory               Factory to use.
   */
  inline constexpr Value::Any internal(Runtime::Isolate *isolate, const Value::Symbol &symbol, Factory &&factory) {
    if (m_internal.contains(symbol)) return m_internal.at(symbol);
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex); // ensure thread-safe
    return m_internal.emplace(symbol, factory(isolate)).first->second;
  }

  /**
   * @brief Handles defining internal natives.
   * @param isolate               Runtime Isolate.
   * @param describe              Symbol to describe.
   * @param callback              Callback to bind.
   */
  Function::Native internal(Runtime::Isolate *isolate, const $::String::View &name, Function::Callback callback);

  /**
   * @brief Ensures that we can set a global variable.
   * @param isolate               Runtime Isolate.
   * @param symbol                Global symbol.
   */
  inline constexpr Value::Any get(Runtime::Isolate *isolate, const Value::Symbol &symbol) {
    if (m_values.contains(symbol)) return m_values.at(symbol);
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex); // ensure thread-safe before continuing
    return m_values.emplace(symbol, m_factories.at(symbol)(isolate)).first->second;
  }
};

} // namespace Sabre::Globals

#endif
