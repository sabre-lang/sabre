#ifndef _SABRE_FUNCTION_ENVIRON_HPP
#define _SABRE_FUNCTION_ENVIRON_HPP

/// Sabre Includes
#include "sabre/forward/function.hpp"
#include "sabre/forward/runtime.hpp"
#include "sabre/iterable/list.hpp"

namespace Sabre::Function {

/// @brief Function Context Environment.
class Environ {
  //  TYPEDEFS  //

  /// @brief Available Context Offsets.
  struct Offset {
    static constexpr size_t PARENT = 0;
    static constexpr size_t VALUES = 1;
  };

  //  PROPERTIES  //

  /// @brief Underlying environment value.
  Value::Any m_value;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a context.
   * @param environment           Environment to bind.
   */
  explicit Environ(const Value::Any &value = Value::Void()) : m_value(value) {}

  /**
   * @brief Constructs a closure context.
   * @param isolate               Runtime isolate.
   * @param size                  Size of context.
   * @param parent                Optional parent.
   */
  explicit Environ(Runtime::Isolate *isolate, size_t size = 0, const Environ &parent = Environ());

  //  OPERATOR METHODS  //

  inline constexpr operator Value::Any() const noexcept { return m_value; }
  inline constexpr operator Iterable::List() const noexcept { return list(); }

  //  PUBLIC METHODS  //

  /// @brief Denotes if a context is valid.
  inline constexpr bool valid() const noexcept { return m_value.is<Iterable::List>(); }

  /// @brief Gets the baseline value.
  inline constexpr Value::Any &environment() noexcept { return m_value; }
  inline constexpr const Value::Any &environment() const noexcept { return m_value; }

  /// @brief Resolves the internal list.
  inline constexpr Iterable::List list() const noexcept { return m_value.as<Iterable::List>(); }

  /// @brief Gets the total size of the context.
  inline constexpr size_t size() const noexcept { return valid() ? list().size() - Offset::VALUES : SIZE_MAX; }

  /// @brief Gets/sets the parent context value.
  inline Environ parent() const noexcept { return Environ(m_get(Offset::PARENT)); }
  inline constexpr void parent(const Iterable::List &next) const noexcept { m_set(Offset::PARENT, next); }

  /**
   * @brief Loads a value from the context.
   * @param index                     Index of value.
   */
  inline constexpr Value::Any load(size_t index) const noexcept { return m_get(Offset::VALUES + index); }

  /**
   * @brief Stores a value on the context.
   * @param index                     Index of value.
   * @param value                     Value to assign.
   */
  inline constexpr void store(size_t index, const Value::Any &value) const noexcept {
    m_set(Offset::VALUES + index, value);
  }

  /**
   * @brief Handles getting a parent at a required depth.
   * @param depth                     Depth of parent.
   */
  inline constexpr Environ parent(size_t depth) const {
    switch (depth) {
    case 0: return *this;    // should be this instance
    case 1: return parent(); // fast-path the immediate parent
    default: {
      auto context = *this; // get the base reference for our depth
      for (; depth && context.valid(); --depth) context = context.parent();
      return context; // return the resulting context as necessary now
    }
    }
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles getting index values.
   * @param index                     Index to retrieve.
   */
  inline constexpr Value::Any m_get(size_t index) const noexcept { return list().get(index); }

  /**
   * @brief Handles setting index values.
   * @param index                     Index of context.
   * @param value                     Value to assign.
   */
  inline constexpr void m_set(size_t index, const Value::Any &value) const noexcept { list().set(index, value); }
};

} // namespace Sabre::Function

#endif
