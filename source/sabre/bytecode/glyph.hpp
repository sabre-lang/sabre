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
    case JUMP_TO: $_FALLTHROUGH;
    case JUMP_FALSEY: $_FALLTHROUGH;
    case JUMP_TRUTHY: $_FALLTHROUGH;
    case JUMP_FILLED: $_FALLTHROUGH;

    case MATCH_VOID: $_FALLTHROUGH;
    case MATCH_TEXT: $_FALLTHROUGH;
    case MATCH_TRUE: $_FALLTHROUGH;
    case MATCH_FALSE: $_FALLTHROUGH;
    case MATCH_CONST: $_FALLTHROUGH;
    case MATCH_GUARD: $_FALLTHROUGH;

    case CALL_0_VOID: $_FALLTHROUGH;
    case CALL_N_VOID: $_FALLTHROUGH;
    case CALL_0_INLINE: $_FALLTHROUGH;
    case CALL_N_INLINE: $_FALLTHROUGH;
    case CALL_0_FIELD: $_FALLTHROUGH;
    case CALL_N_FIELD: $_FALLTHROUGH;

    case SPAWN_0_VOID: $_FALLTHROUGH;
    case SPAWN_N_VOID: $_FALLTHROUGH;
    case SPAWN_0_FIELD: $_FALLTHROUGH;
    case SPAWN_N_FIELD: $_FALLTHROUGH;

    case CLASS_BIND: $_FALLTHROUGH;
    case CLASS_SUPER: $_FALLTHROUGH;
    case CLASS_EXPORT: $_FALLTHROUGH;
    case CLOSURE_PASS: $_FALLTHROUGH;

    case DISPOSE_CLOSE: $_FALLTHROUGH;

    case ITER_LOAD: $_FALLTHROUGH;
    case ITER_NEXT: $_FALLTHROUGH;

    case MODULE_BARREL: $_FALLTHROUGH;
    case MODULE_IMPORT: $_FALLTHROUGH;
    case MODULE_EXPORT: $_FALLTHROUGH;

    case OBJECT_ATTR: $_FALLTHROUGH;

    case LOAD_CONTEXT: $_FALLTHROUGH;
    case LOAD_FIELD: $_FALLTHROUGH;
    case LOAD_UPVALUE: $_FALLTHROUGH;

    case STORE_CONTEXT: $_FALLTHROUGH;
    case STORE_FIELD: $_FALLTHROUGH;
    case STORE_UPVALUE: $_FALLTHROUGH;

    case UNOP_NEG: $_FALLTHROUGH;
    case UNOP_INV: $_FALLTHROUGH;

    case BINOP_ADD: $_FALLTHROUGH;
    case BINOP_SUB: $_FALLTHROUGH;
    case BINOP_MUL: $_FALLTHROUGH;
    case BINOP_DIV: $_FALLTHROUGH;
    case BINOP_MOD: $_FALLTHROUGH;
    case BINOP_POW: $_FALLTHROUGH;
    case BINOP_XOR: $_FALLTHROUGH;
    case BINOP_BOR: $_FALLTHROUGH;
    case BINOP_BAND: $_FALLTHROUGH;
    case BINOP_SHL: $_FALLTHROUGH;
    case BINOP_SHR: $_FALLTHROUGH;

    case BINOP_ADDI: $_FALLTHROUGH;
    case BINOP_SUBI: $_FALLTHROUGH;
    case BINOP_MULI: $_FALLTHROUGH;
    case BINOP_DIVI: $_FALLTHROUGH;
    case BINOP_MODI: $_FALLTHROUGH;
    case BINOP_POWI: $_FALLTHROUGH;
    case BINOP_XORI: $_FALLTHROUGH;
    case BINOP_BORI: $_FALLTHROUGH;
    case BINOP_BANDI: $_FALLTHROUGH;
    case BINOP_SHLI: $_FALLTHROUGH;
    case BINOP_SHRI: $_FALLTHROUGH;

    case EXEC_PANIC: return true;

    // otherwise default to non-panicking
    default: return false;
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
