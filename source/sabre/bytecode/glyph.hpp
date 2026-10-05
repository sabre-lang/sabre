#ifndef _SABRE_BYTECODE_GLYPH_HPP
#define _SABRE_BYTECODE_GLYPH_HPP

/// Sabre Includes
#include "sabre/engine/operand.hpp"

/// Forward Definitions
$_FWD(Sabre::Bytecode::Constants, static constexpr uint8_t GLYPHS_LAST = 0b1 << 7)
$_FWD(Sabre::Bytecode::Constants, static constexpr uint8_t GLYPHS_MASK = ~GLYPHS_LAST)

namespace Sabre::Bytecode {

/// @brief Encapsulates Bytecode Glyphs.
struct Glyph final : public Engine::Operand<1> {
  //  TYPEDEFS  //

  /// @brief The internal enum values.
  enum Encoded : uint8_t {
#define SABRE_XX_GLYPH_BASE(N, ...) N,
#include "sabre/bytecode/_defines/glyphs.def"
  };

private:
  //  PROPERTIES  //

  /// @brief The internal encoded value.
  Encoded m_value = EXEC_INVALID;

public:
  //  CONSTRUCTORS  //

  /// @brief Allow default construction.
  constexpr Glyph() = default;

  /// @brief Allow copy/move constructors/
  constexpr Glyph(const Encoded &value) : m_value(value) {}
  constexpr Glyph(Encoded &&value) : m_value(std::move(value)) {}

  //  OPERATOR METHODS  //

  inline constexpr bool operator==(Encoded value) const noexcept { return m_value == value; }
  inline constexpr bool operator==(const Glyph &other) const noexcept { return m_value == other.m_value; }

  //  PUBLIC METHODS  //

  /// @brief Gets the encoded glyph value.
  inline constexpr Encoded encoded() const noexcept { return static_cast<Encoded>(m_value); }

  /// @brief Gets the associated glyph name.
  inline constexpr $::String::View label() const noexcept {
    switch (encoded()) {
#define SABRE_XX_GLYPH_BASE(N, ...) \
  case N: return #N;
#include "sabre/bytecode/_defines/glyphs.def"
    default: return "MISC_UNKNOWN";
    }
  }

  /// @brief Denotes if an incoming target branches.
  inline constexpr bool branches() const noexcept {
    switch (encoded()) {
#define SABRE_XX_GLYPH_JUMP(N, ...) \
  case N: return true;
#include "sabre/bytecode/_defines/glyphs.def"
    default: return false;
    }
  }

  /// @brief Denotes if an operation is indexed (eg: fast-ops)
  inline constexpr bool indexed() const noexcept {
    switch (encoded()) {
#define SABRE_XX_GLYPH_INDEXED(P, N, ...) \
  case P##_##N##I: return true;
#include "sabre/bytecode/_defines/glyphs.def"
    default: return false;
    }
  }

  /// @brief Denotes if this is a terminating instruction.
  inline constexpr bool terminates() const noexcept {
    switch (encoded()) {
    case EXEC_ABORT: $_FALLTHROUGH;
    case EXEC_PANIC: $_FALLTHROUGH;
    case EXEC_RETURN: return true;
    default: return false;
    }
  }

  /// @brief Denotes if this a leaking instruction (eg: upvalues).
  inline constexpr bool leaked() const noexcept {
    switch (encoded()) {
    case LOAD_CONTEXT: $_FALLTHROUGH;
    case LOAD_UPVALUE: $_FALLTHROUGH;
    case STORE_CONTEXT: $_FALLTHROUGH;
    case STORE_UPVALUE: return true;
    default: return false;
    }
  }

  /// @brief Denotes if this instruction could panic.
  inline constexpr bool panics() const noexcept {
    switch (encoded()) {
    case LOAD_ZERO: $_FALLTHROUGH;
    case LOAD_ONE: $_FALLTHROUGH;
    case LOAD_VOID: $_FALLTHROUGH;
    case LOAD_TRUE: $_FALLTHROUGH;
    case LOAD_FALSE: $_FALLTHROUGH;
    case LOAD_SELF: $_FALLTHROUGH;
    case LOAD_CONST: $_FALLTHROUGH;
    case LOAD_GLOBAL: $_FALLTHROUGH;

    case LOAD_CONTEXT: $_FALLTHROUGH;
    case LOAD_UPVALUE: $_FALLTHROUGH;
    case STORE_CONTEXT: $_FALLTHROUGH;
    case STORE_UPVALUE: $_FALLTHROUGH;

    case REG_SWAP: $_FALLTHROUGH;
    case REG_MOVE: $_FALLTHROUGH;

    case TYPE_GUARD: $_FALLTHROUGH;

    case DISPOSE_OPEN: $_FALLTHROUGH;
    case DISPOSE_CLOSE: $_FALLTHROUGH;
    case DISPOSE_TRACE: $_FALLTHROUGH;

    case MODULE_OPEN: $_FALLTHROUGH;
    case MODULE_CLOSE: $_FALLTHROUGH;

    case CLASS_MAKE: $_FALLTHROUGH;

    case CLOSURE_MAKE: $_FALLTHROUGH;
    case CLOSURE_LIFT: $_FALLTHROUGH;

    case ENUM_MAKE: $_FALLTHROUGH;
    case ENUM_EMPTY: $_FALLTHROUGH;

    case LIST_MAKE: $_FALLTHROUGH;
    case LIST_EMPTY: $_FALLTHROUGH;

    case OBJECT_MAKE: $_FALLTHROUGH;
    case OBJECT_EMPTY: $_FALLTHROUGH;

    case STRING_MAKE: $_FALLTHROUGH;
    case STRING_CONCAT: $_FALLTHROUGH;

    case UNOP_NOT: $_FALLTHROUGH;
    case BINOP_COAL: $_FALLTHROUGH;

    case TEST_EQ: $_FALLTHROUGH;
    case TEST_NE: $_FALLTHROUGH;

    case TEST_GT: $_FALLTHROUGH;
    case TEST_LT: $_FALLTHROUGH;
    case TEST_GE: $_FALLTHROUGH;
    case TEST_LE: $_FALLTHROUGH;

    case TEST_GTI: $_FALLTHROUGH;
    case TEST_LTI: $_FALLTHROUGH;
    case TEST_GEI: $_FALLTHROUGH;
    case TEST_LEI: $_FALLTHROUGH;

    case EXEC_RETURN: $_FALLTHROUGH;
    case EXEC_ABORT: $_FALLTHROUGH;
    case EXEC_RAISE: $_FALLTHROUGH;
    case EXEC_NOOP: return false;

    // otherwise default to panicking
    default: return true;
    }
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles printing glyphs.
   * @param os                      Output stream.
   * @param self                    Glyph instance.
   */
  static inline void m_print(std::ostream &os, const Glyph &self) { os << self.label(); }
};

/// @brief Ensures that the maximum glyph does not exceed 1-byte in size.
static_assert(static_cast<uint8_t>(Glyph::EXEC_INVALID) < Constants::GLYPHS_LAST);

} // namespace Sabre::Bytecode

#endif
