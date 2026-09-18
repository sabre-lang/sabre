#ifndef _SABRE_VARIABLES_SCOPE_HPP
#define _SABRE_VARIABLES_SCOPE_HPP

/// Sabre Includes
#include "sabre/bytecode/allocator.hpp"
#include "sabre/value/void.hpp"
#include "sabre/variable/upvalues.hpp"

namespace Sabre::Variable {

/// @brief Variables Context Scoping.
class Scope {
  //  PROPERTIES  //

  /// @brief Handles binding leaked values.
  $::Map::Set<$::String::View> m_leaked = {};

  /// @brief Currently bound locals.
  $::Map::Record<Register::Slot> m_bound = {};

  /// @brief Available constant variables.
  $::Map::Record<Value::Any> m_constants = {};

  /// @brief Previous Variables Instance.
  $::Shared::Pointer<Scope> m_ancestor = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a scoped set of variables.
   * @param ancestor              Variables ancestor.
   */
  explicit Scope(const $::Shared::Pointer<Scope> &ancestor = nullptr) : m_ancestor(ancestor) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the variables ancestor value.
  inline constexpr const $::Shared::Pointer<Scope> &ancestor() const noexcept { return m_ancestor; }

  /**
   * @brief Attempts checking if there is a bound local.
   * @param name                  Name of local.
   */
  inline constexpr bool contains(const $::String::View &name) const {
    if (m_bound.contains(name)) return true;
    return m_ancestor ? m_ancestor->contains(name) : false;
  }

  /**
   * @brief Attempts checking if there is a leaked local.
   * @param name                  Name of local.
   */
  inline constexpr bool leaked(const $::String::View &name) const {
    if (m_leaked.contains(name)) return true;
    return m_ancestor ? m_ancestor->leaked(name) : false;
  }

  /**
   * @brief Resolves a local or upper-scoped variable.
   * @param name                  Name of local.
   */
  inline Register::Slot resolve(const $::String::View &name) const {
    if (m_bound.contains(name)) return m_bound.at(name);
    return m_ancestor ? m_ancestor->resolve(name) : Register::Slot();
  }

  /**
   * @brief Resolves a local or upper-scoped constant.
   * @param name                  Name of local.
   */
  inline Value::Any constant(const $::String::View &name) const {
    if (m_constants.contains(name)) return m_constants.at(name);
    return m_ancestor ? m_ancestor->constant(name) : Value::Failure();
  }

  /**
   * @brief Resolves a contextual upvalue.
   * @param name                      Name of upvalue.
   */
  inline Upvalue upvalue(const $::String::View &name) const { return m_upvalue(name); }

  /**
   * @brief Handles declaring a variable.
   * @param name                      Name of variable.
   * @param vreg                      Variable register.
   * @param leaked                    If variable is leaked.
   */
  inline bool declare(const $::String::View &name, const Register::Slot &vreg, bool leaked = false) {
    if (leaked) m_leaked.emplace(name); // emplace
    return m_bound.try_emplace(name, vreg).second;
  }

  /**
   * @brief Handles declaring a constant.
   * @param name                      Name of varible.
   * @param value                     Value to bind.
   */
  inline bool declare(const $::String::View &name, const Value::Any &value) {
    return m_constants.try_emplace(name, value).second;
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles resolving upvalues.
   * @param name                      Name of upvalue.
   * @param depth                     Current depth.
   */
  Upvalue m_upvalue(const $::String::View &name, size_t depth = 0) const;
};

} // namespace Sabre::Variable

#endif
