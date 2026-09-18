#ifndef _SABRE_BYTECODE_ITERATOR_HPP
#define _SABRE_BYTECODE_ITERATOR_HPP

/// Sabre Includes
#include "sabre/bytecode/instruction.hpp"
#include "sabre/image/slice.hpp"

namespace Sabre::Bytecode {

/**
 * @brief Bytecode Iterator Span.
 * @param slice                 Bytecode slice.
 */
static inline constexpr std::span<const Instruction> Iterator(const Image::Slice &slice) {
  return {reinterpret_cast<const Instruction *>(slice.address()), slice.size() / Width};
}

} // namespace Sabre::Bytecode

#endif
