#ifndef _SABRE_BYTECODE_INSTRUCTION_HPP
#define _SABRE_BYTECODE_INSTRUCTION_HPP

/// Vendor Modules
#include <xlsp/xlsp.hpp>

/// Sabre Modules
#include "sabre/bytecode/allocator.hpp"
#include "sabre/bytecode/glyph.hpp"
#include "sabre/bytecode/label.hpp"
#include "sabre/bytecode/operands.hpp"

//  MACROS  //

/// @brief Helper for constructing bytecode instructions.
#define SABRE_MM_BIR(N, ...) ::Sabre::Bytecode::Qualified<::Sabre::Bytecode::Glyph::N>(__VA_ARGS__)

//  NAMESPACES  //

namespace Sabre::Bytecode {

/// @brief Boxed Instruction Typing.
using Boxed = $::Unique::Pointer<Instruction>;

/// @brief Bytecode View Container.
template <Glyph::Encoded G> class Qualified;

/// @brief Base Instruction Container.
struct Instruction {
  //  TYPEDEFS  //

  /// @brief The encoded instruction typing.
  using Encoded = uint64_t;

protected:
  //  PROPERTIES  //

  /// @brief The underlying encoded value.
  Encoded m_encoded;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an instruction.
   * @param glyph               Glyph to bind.
   */
  constexpr Instruction() : Instruction(Glyph::EXEC_NOOP) {}
  constexpr Instruction(const Glyph &glyph) : m_encoded(glyph.encoded()) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying instruction.
  inline constexpr Encoded encode() const noexcept { return m_encoded; }

  /// @brief Gets the underlying syllable value.
  inline constexpr Glyph glyph() const noexcept {
    return static_cast<Glyph::Encoded>(m_encoded & Constants::GLYPHS_MASK);
  }

  /// @brief Checks if the debug breakpoint is set.
  inline constexpr bool breakpoint() const noexcept {
    return (m_encoded & Constants::GLYPHS_MASK) == Constants::GLYPHS_MASK;
  }

  /// @brief Converts the instruction to a valid view.
  template <Glyph::Encoded G> inline constexpr Qualified<G> *cast() noexcept {
    return static_cast<Qualified<G> *>(this);
  }

  /// @brief Converts the instruction to a valid view.
  template <Glyph::Encoded G> inline constexpr const Qualified<G> *cast() const noexcept {
    return static_cast<const Qualified<G> *>(this);
  }

  /// @brief Gets the associated instruction name.
  inline constexpr $::String::View label() const noexcept { return glyph().label(); }

  /// @brief Denotes if an incoming target branches.
  inline constexpr bool branches() const noexcept { return glyph().branches(); }

  /// @brief Denotes if this is a terminating instruction.
  inline constexpr bool terminates() const noexcept { return glyph().terminates(); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles printing instructions.
   * @param os                    Output stream.
   * @param self                  Instruction value.
   */
  static void m_print(std::ostream &os, const Instruction &self);
};

/// @brief Qualified Instruction View.
template <Glyph::Encoded G> class Qualified : public Instruction {
  //  TYPEDEFS  //

  /// @brief Allow instructions internal access.
  friend struct Instruction;

  /// @brief Arguments Typing View.
  template <Glyph::Encoded> struct Arguments;

#define SABRE_XX_GLYPH_BASE(N, ...)                                         \
  template <> struct Arguments<Glyph::N> : public Operands<__VA_ARGS__> {};
#include "sabre/bytecode/_defines/glyphs.def"

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Handles constructing a qualified instruction.
   * @param operands              Operands to assign.
   */
  template <class... As> constexpr Qualified(As &&...operands) : Instruction(G) {
    m_unpack(std::make_index_sequence<sizeof...(As)>(), std::forward<As>(operands)...);
  }

  //  PUBLIC METHODS  //

  /// @brief Gets an instruction operand at the given index.
  template <size_t I> inline constexpr auto get() const noexcept {
    using T = Arguments<G>::template Element<I>;
    auto [offset, mask] = Arguments<G>::template traits<I>();
    return T((m_encoded >> (sizeof(Glyph) + offset) * 8) & mask);
  }

  /**
   * @brief Sets an operand at the given index.
   * @param value                 Value to assign.
   */
  template <size_t I> inline constexpr void set(const Arguments<G>::template Element<I> &value) {
    auto [offset, mask] = Arguments<G>::template traits<I>(); // prepare
    m_encoded |= (value.encode() & mask) << (sizeof(Glyph) + offset) * 8;
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles unpacking multiple operands to be set.
   * @param operands              Operands to unpack.
   */
  template <class... As, size_t... Is> inline constexpr void m_unpack(std::index_sequence<Is...>, As &&...operands) {
    ((set<Is>(operands)), ...); // unpack all the incoming operands
  }

  /**
   * @brief Handles printing the qualified instruction.
   * @param os                    Output stream.
   */
  template <size_t... Is> inline constexpr void m_print(std::ostream &os, std::index_sequence<Is...>) const noexcept {
    (..., (os << (Is == 0 ? " " : ", ") << get<Is>()));
  }

  /**
   * @brief Handles printing the qualified instruction.
   * @param os                    Output stream.
   */
  inline constexpr void m_print(std::ostream &os) const noexcept { m_print(os, Arguments<G>::sequence()); }
};

/// @brief Prepare a baseline width to be used.
static constexpr size_t Width = sizeof(Instruction);

// ensure all instructions are validly encoded
static_assert(Width == sizeof(Instruction::Encoded));

// and ensure that the underlying qualified instructions are too
#define SABRE_XX_GLYPH_BASE(N, ...) static_assert(sizeof(Qualified<Glyph::N>) == Width);
#include "sabre/bytecode/_defines/glyphs.def"

} // namespace Sabre::Bytecode

#endif
