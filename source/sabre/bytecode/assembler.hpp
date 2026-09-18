#ifndef _SABRE_BYTECODE_ASSEMBLER_HPP
#define _SABRE_BYTECODE_ASSEMBLER_HPP

/// Sabre Includes
#include "sabre/bytecode/block.hpp"
#include "sabre/image/arena.hpp"

namespace Sabre::Bytecode {

/// @brief Bytecode Assembler Service.
class Assembler : public XI::Transient {
  //  TYPEDEFS  //

  /// @brief Allow the compiler internal access.
  friend class Compiler;

  //  PROPERTIES  //

  /// @brief Current top-most label.
  Label m_head = 0;

  /// @brief Available source positions.
  Mapping m_positions = {};

  /// @brief All labels that have been placed.
  Label::Placed m_placed = {};

  /// @brief All labels that are unresolved.
  Label::Unresolved m_unresolved = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a bytecode assembler.
  explicit Assembler();
  explicit Assembler(XI::Container *services);

  //  PUBLIC METHODS  //

  /**
   * @brief Handles assembling a routine.
   * @param routine                   Routine to assemble.
   * @param arena                     Bytecode output arena.
   */
  Image::Slice process(Routine *routine, Image::Arena *arena);

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles patching all unresolved labels.
   * @param binary                    Binary to patch.
   * @param arena                     Arena to be patched.
   */
  void m_patch(Image::Binary &binary, Image::Arena *arena) const;

  /**
   * @brief Handles encoding instructions.
   * @param instruction               Instruction to encode.
   * @param offset                    Current offset for the instruction.
   */
  uint64_t m_encode(const Instruction *instruction, Label::Encoded offset);
};

} // namespace Sabre::Bytecode

#endif
