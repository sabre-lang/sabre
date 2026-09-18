#ifndef _SABRE_OBJECT_ANY_HPP
#define _SABRE_OBJECT_ANY_HPP

/// Sabre Includes
#include "sabre/object/header.hpp"

namespace Sabre::Object {

/// @brief Dynamic Object Wrapper.
struct Any : public Value::Any {
  //  TYPEDEFS  //

  /// @brief Allow the factory internal access.
  friend struct Allocator;

  /// @brief Allow the mixin internal access.
  template <class T> friend struct Mixin;

  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an object instance.
   * @param pointer               Object pointer.
   */
  constexpr Any() : Any(m_encode(0)) {}
  constexpr Any(Pointer::Underlying pointer) : Value::Any(pointer) {}

  //  PUBLIC METHODS  //

  inline constexpr bool nullish() const noexcept { return address() == 0; }
  inline constexpr Header *header() const noexcept { return m_header(m_pointer); }
  inline constexpr Pointer::Underlying address() const noexcept { return m_decode(m_pointer); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Validates the value.
   * @param pointer               Pointer to validate.
   */
  static inline constexpr bool m_is(const Pointer::Tagged &pointer) {
    return pointer.is<Pointer::Kind::JUN, Pointer::Kind::SEN>();
  }

  /**
   * @brief Handles decoding object headers.
   * @param pointer               Tagged pointer.
   */
  static inline constexpr Header *m_header(Pointer::Underlying pointer) {
    return std::bit_cast<Header *>(m_decode(pointer) - Pointer::Alignment::OBJECT);
  }

  /**
   * @brief Handles encoding object values.
   * @param address               Object address.
   * @param senior                Senior flag.
   */
  static inline constexpr Pointer::Underlying m_encode(Pointer::Underlying address, bool senior = false) {
    return address | static_cast<Pointer::Underlying>(Pointer::Kind::OBJ) | (senior << Pointer::Shift::OBJ);
  }

  /**
   * @brief Handles decoding object addresses.
   * @param pointer               Tagged pointer.
   */
  static inline constexpr Pointer::Underlying m_decode(Pointer::Underlying pointer) {
    $_ASSERT(m_is(pointer), "Invalid 'object' decode value");
    return pointer & ~Pointer::Mask::IMM; // get the pointer
  }

  /**
   * @brief Handles dumping the runtime value.
   * @param os                    Output stream.
   * @param self                  Value instance.
   */
  static inline void m_print(std::ostream &os, const Any &self) {
    os << $::Dye::cyan("<{0}: 0x{1:08X}>", self.brand(), self.m_pointer.value());
  }
};

} // namespace Sabre::Object

#endif
