#ifndef _SABRE_TYPE_ENTITY_HPP
#define _SABRE_TYPE_ENTITY_HPP

/// Sabre Includes
#include "sabre/type/registry.hpp"
#include "sabre/variable/modifiers.hpp"

/// Type Includes
#include "sabre/type/utility/intrinsics.hpp"

namespace Sabre::Type {

/// @brief Encapsulates a type/value pairing.
class $_ALIGNAS_CACHE() Entity {
  //  PROPERTIES  //

  /// @brief The current bound type.
  Erased m_type = New::unset();

  /// @brief The current bound value.
  Erased m_value = New::unset();

  /// @brief The current deprecation notice.
  $::String::View m_notice = {};

  /// @brief Bound variable modifiers.
  Variable::Modifiers m_modifiers = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted type-entity.
  constexpr Entity() = default;

  /**
   * @brief Constructs an entity.
   * @param value                 Value reference.
   * @param modifiers             Variable modifiers.
   */
  template <std::derived_from<Unit> T>
  constexpr Entity(const $::Shared::Pointer<T> &value, Variable::Modifiers modifiers = {}) :
      m_value(value), m_modifiers(modifiers) {}

  /**
   * @brief Constructs an entity.
   * @param value                 Value reference.
   * @param type                  Type reference.
   * @param modifiers             Variable modifiers.
   */
  constexpr Entity(const Erased &value, const Erased &type, Variable::Modifiers modifiers = {}) :
      m_type(type), m_value(value), m_modifiers(modifiers) {}

  //  PUBLIC METHODS  //

  inline constexpr bool opaque() const noexcept { return !m_value->is<Unset>(); }
  inline constexpr bool transient() const noexcept { return !m_type->is<Unset>(); }
  inline constexpr bool unset() const noexcept { return m_value->is<Unset>() && m_type->is<Unset>(); }

  /// @brief Gets the unused state of the entity.
  inline constexpr bool unused(bool state) noexcept { return m_modifiers.flip(Variable::Flag::OWNED, !state), !state; }
  inline constexpr bool unused() const noexcept { return !m_modifiers.test(Variable::Flag::OWNED) && !exported(); }

  /// @brief Gets the type assigned to the entity.
  inline constexpr Erased &type() noexcept { return m_type; }
  inline constexpr const Erased &type() const noexcept { return m_type; }

  /// @brief Gets the value assigned to the entity.
  inline constexpr Erased &value() noexcept { return m_value; }
  inline constexpr const Erased &value() const noexcept { return m_value; }

  /// @brief Gets an entities variable modifiers.
  inline constexpr Variable::Modifiers &modifiers() noexcept { return m_modifiers; }
  inline constexpr const Variable::Modifiers &modifiers() const noexcept { return m_modifiers; }

  /// @brief Gets the internal deprecation notice.
  inline constexpr $::String::View notice() const noexcept { return m_notice; }

  /// @brief Gets the current deprecation state.
  inline constexpr bool deprecated() const noexcept { return m_modifiers.test(Variable::Flag::NOTICE); }
  inline constexpr bool deprecated(const $::String::View &notice) noexcept {
    return m_modifiers.set(Variable::Flag::NOTICE), m_notice = notice, true;
  }

  /// @brief Gets specific flag details of the entity.
  inline constexpr bool exported() const noexcept { return m_modifiers.test(Variable::Flag::EXPORT); }
  inline constexpr bool immutable() const noexcept { return !m_modifiers.test(Variable::Flag::MUTABLE); }
  inline constexpr bool optional() const noexcept { return m_modifiers.test(Variable::Flag::OPTIONAL); }

  /**
   * @brief Handles inferring an entities details.
   * @param constraints                 Generic constraints.
   */
  inline constexpr Entity infer(Constraints *constraints = nullptr) const noexcept {
    // prepare the clone to be used now
    auto inferred = Entity(*this);

    // otherwise we should update our base types now
    inferred.m_type = m_type->infer(constraints);
    inferred.m_value = m_value->infer(constraints);

    // and return the resulting clone of the entity now
    return inferred;
  }
};

} // namespace Sabre::Type

#endif
