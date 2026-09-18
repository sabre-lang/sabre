#ifndef _SABRE_BYTECODE_LABEL_HPP
#define _SABRE_BYTECODE_LABEL_HPP

/// Sabre Includes
#include "sabre/engine/operand.hpp"

namespace Sabre::Bytecode {

/// @brief Explicit Index Typing.
struct Index : public Engine::Operand<2>, public $::Unit::Strong<uint16_t, Index> {
  //  TYPEDEFS  //

  /// @brief The encoded typing.
  using Encoded = decltype(m_value);

  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructors.
  using Strong::Strong;

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying encoded value.
  inline constexpr Encoded encode() const noexcept { return m_value; }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles printing index values.
   * @param os                    Output stream.
   * @param self                  Index value.
   */
  static inline void m_print(std::ostream &os, const Index &self) { os << '[' << self.encode() << ']'; }
};

/// @brief Explicit Label Typing.
struct Label : public Index {
  //  TYPEDEFS  //

  /// @brief Unresolved label patch.
  using Patch = std::pair<Index, Label>;

  /// @brief Placed labels container.
  using Placed = $::Map::Base<Label, Encoded>;

  /// @brief Unresolved labels container.
  using Unresolved = $::Map::Base<Encoded, Patch>;

  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructors.
  using Index::Index;

  /**
   * @brief Casts indicies to labels.
   * @param index                 Index to cast.
   */
  constexpr Label(const Index &index) : Index(index) {}
  constexpr Label(Index &&index) : Index(std::move(index)) {}

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles printing label values.
   * @param os                    Output stream.
   * @param self                  Label value.
   */
  static inline void m_print(std::ostream &os, const Label &self) { os << ".L" << self.encode(); }
};

} // namespace Sabre::Bytecode

//  SPECIALIZATIONS  //

template <> struct ankerl::unordered_dense::hash<Sabre::Bytecode::Index> {
  using is_avalanching = void; // declare that this is not avalanchable
  $_NODISCARD uint64_t operator()(const Sabre::Bytecode::Index &index) const noexcept { return index.encode(); }
};

template <> struct ankerl::unordered_dense::hash<Sabre::Bytecode::Label> {
  using is_avalanching = void; // declare that this is not avalanchable
  $_NODISCARD uint64_t operator()(const Sabre::Bytecode::Label &label) const noexcept { return label.encode(); }
};

#endif
