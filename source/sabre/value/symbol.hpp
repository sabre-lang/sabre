#ifndef _SABRE_VALUE_SYMBOL_HPP
#define _SABRE_VALUE_SYMBOL_HPP

/// Sabre Includes
#include "sabre/value/common.hpp"

namespace Sabre::Value {

/// @brief 60-bit hash-value.
using Hash = $_ARCH_TYPED(uint64_t, uint32_t);

/// @brief Symbol Runtime Value.
struct Symbol : public Any {
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted symbol value.
  constexpr Symbol() : Any(m_encode(0)) {}

  /**
   * @brief Constructs a symbol from a hash.
   * @param hash                  Hash to encapsulate.
   */
  constexpr Symbol(Hash hash) : Any(m_encode(hash)) {}

  /**
   * @brief Constructs a symbol from a string-value.
   * @param buffer                String to pre-hash.
   */
  constexpr Symbol(const char *buffer) : Symbol(buffer, std::strlen(buffer)) {}
  constexpr Symbol(const char *buffer, size_t size) : Symbol($::String::View(buffer, size)) {}

  /**
   * @brief Constructs a symbol from a string-value.
   * @param view                  String to pre-hash.
   */
  constexpr Symbol(const $::String::View &view) : Any(m_encode(XH::WY::XX(view))) {}

  /**
   * @brief Constructs a symbol from a resource.
   * @param resource              Document resource.
   */
  constexpr Symbol(const $::URI::View &resource) : Symbol(resource.view()) {}
  constexpr Symbol(const $::URI::Buffer &resource) : Symbol(resource.view()) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying hash-value.
  inline constexpr Hash hash() const noexcept { return m_decode(m_pointer); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Get truthiness of a value.
   * @param self                Value to resolve.
   */
  static inline constexpr bool m_truthiness(const Symbol &) { return true; }

  /**
   * @brief Validates the value.
   * @param pointer             Tagged pointer.
   */
  static inline constexpr bool m_is(const Pointer::Tagged &pointer) { return pointer.is<Pointer::Kind::SYM>(); }

  /**
   * @brief Handles encoding symbol hashes.
   * @param hash                  Symbol hash.
   */
  static inline constexpr Pointer::Underlying m_encode(Hash hash) {
    return (hash << Pointer::Shift::SYM) | static_cast<Pointer::Underlying>(Pointer::Kind::SYM);
  }

  /**
   * @brief Handles decoding symbol hashes.
   * @param pointer               Pointer to decode.
   */
  static inline constexpr Hash m_decode(Pointer::Underlying pointer) {
    $_ASSERT(m_is(pointer), "Invalid 'Symbol' decode value");
    return pointer >> Pointer::Shift::SYM; // and shift
  }

  /**
   * @brief Handles dumping the runtime value.
   * @param os                    Output stream.
   * @param self                  Value instance.
   */
  static inline void m_print(std::ostream &os, const Symbol &self) { os << $::Dye::green("Symbol({0})", self.hash()); }
};

} // namespace Sabre::Value

//  SPECIALIZATIONS  //

template <> struct ankerl::unordered_dense::hash<Sabre::Value::Symbol> {
  using is_avalanching = void; // declare that this is not avalanchable
  $_NODISCARD uint64_t operator()(const Sabre::Value::Symbol &symbol) const noexcept { return symbol.hash(); }
};

#endif
