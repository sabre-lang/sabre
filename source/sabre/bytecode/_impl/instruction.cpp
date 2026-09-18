/// Sabre Modules
#include "sabre/bytecode/instruction.hpp"

//  PRIVATE METHODS  //

void Sabre::Bytecode::Instruction::m_print(std::ostream &os, const Instruction &self) {
  // print the baseline name to begin with
  os << self.label();

  // attempt printing each available operand now
  switch (self.glyph().encoded()) {
#define SABRE_XX_GLYPH_BASE(N, ...)                         \
  case Glyph::N: self.cast<Glyph::N>()->m_print(os); break;
#include "sabre/bytecode/_defines/glyphs.def"
  }
}
