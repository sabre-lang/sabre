#ifndef _SABRE_MEMBER_REFERENCE_HPP
#define _SABRE_MEMBER_REFERENCE_HPP

/// Sabre Includes
#include "sabre/member/descriptor.hpp"
#include "sabre/value/void.hpp"

namespace Sabre::Member {

/// @brief Reference Member Descriptor.
class Reference : public Descriptor {
  //  PROPERTIES  //

  /// @brief The underlying reference value.
  Value::Any m_reference = {};

  /// @brief The underlying reference key.
  $::String::Buffer m_key = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Do not allow constructing empty references.
  explicit Reference() = delete;

  /**
   * @brief Constructs an initial reference.
   * @param key               Reference key.
   * @param readonly          Readonly state.
   */
  explicit Reference(const $::String::View &key, bool readonly = false) : Descriptor(readonly), m_key(key) {}

  /**
   * @brief Constructs a member reference.
   * @param key               Reference key.
   * @param value             Reference value.
   * @param readonly          Readonly state.
   */
  explicit Reference(const $::String::View &key, const Value::Any &value, bool readonly = false) :
      Descriptor(readonly), m_reference(value), m_key(key) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying reference key.
  inline constexpr $::String::View key() const noexcept { return m_key; }

  /// @brief Gets the underlying reference value.
  inline constexpr Value::Any &value() noexcept { return m_reference; }
  inline constexpr Value::Any value() const noexcept { return m_reference; }

  /**
   * @brief Handles getting the value.
   * @param isolate           Runtime isolate.
   * @param self              Self value.
   */
  Value::Any getter(Runtime::Isolate *isolate, const Value::Any &self) const final;

  /**
   * @brief Handles setting the value.
   * @param isolate           Runtime isolate.
   * @param self              Self value.
   * @param value             Value to assign.
   */
  Value::Any setter(Runtime::Isolate *isolate, const Value::Any &self, const Value::Any &value) final;
};

} // namespace Sabre::Member

#endif
