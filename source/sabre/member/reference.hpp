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

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an initial reference.
   * @param readonly          Readonly state.
   */
  explicit Reference(bool readonly = false) : Descriptor(readonly) {}

  /**
   * @brief Constructs a member reference.
   * @param reference         Reference value.
   * @param readonly          Readonly state.
   */
  explicit Reference(const Value::Any &reference, bool readonly = false) :
      Descriptor(readonly), m_reference(reference) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying reference value.
  inline constexpr Value::Any &reference() noexcept { return m_reference; }
  inline constexpr Value::Any reference() const noexcept { return m_reference; }

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
