#ifndef _SABRE_BYTECODE_OPERANDS_HPP
#define _SABRE_BYTECODE_OPERANDS_HPP

/// Sabre Includes
#include "sabre/forward/bytecode.hpp"

namespace Sabre::Bytecode {

/// @brief Bytecode Operands Metadata.
template <class... Os> struct Operands : private $::Ensure::Static {
  //  TYPEDEFS  //

  /// @brief Packed set of operands.
  using Packed = std::tuple<Os...>;

  /// @brief Gets the type of an operand element.
  template <size_t I> using Element = std::tuple_element_t<I, Packed>;

  //  PUBLIC METHODS  //

  /// @brief Allows validating operands.
  template <size_t I, class T> static inline constexpr bool is() { return std::same_as<T, Element<I>>; }

  /// @brief Gets the total argument count.
  static inline constexpr size_t argc() { return sizeof...(Os); }

  /// @brief Gets the total size of the operands.
  static inline constexpr size_t size() { return ((sizeof(Os)) + ...); }

  /// @brief Constructs a suitable index-sequence.
  static inline constexpr auto sequence() { return std::make_index_sequence<argc()>(); }

  /// @brief Gets the traits (offset, mask) for an operand.
  template <size_t I> static inline constexpr std::pair<uint64_t, uint64_t> traits() {
    return {offset<I>(), (2 << (8 * Element<I>::m_sizeof() - 1)) - 1};
  }

  /// @brief Gets the byte offset of an operand.
  template <size_t I> static inline constexpr uint64_t offset() {
    if constexpr (I == 0) return 0; // base case value
    else return offset<I - 1>() + Element<I - 1>::m_sizeof();
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Allows validating incoming arguments.
  template <class T>
  static inline constexpr bool m_validate = requires(T t) {
    { t.encode() } -> std::convertible_to<uint64_t>;
  };

  /// @brief Ensure that all the operands can be encoded.
  static_assert(sizeof...(Os) ? ((m_validate<Os>) || ...) : true, "Operands not encodable");
};

} // namespace Sabre::Bytecode

#endif
