/// Sabre Modules
#include "sabre/bytecode/block.hpp"

//  PRIVATE METHODS  //

void Sabre::Bytecode::Block::m_print(std::ostream &os, const Block &self) {
  // show the head of the block now as necessary
  os << "  | #" << fmt::format("{0} {1} ", fmt::ptr(&self), fmt::join(self.m_labels, " "));

  os << "i[ "; // incoming blocks now
  for (const auto &block : self.m_incoming) os << '#' << fmt::format("{0}", fmt::ptr(block)) << ' ';

  os << "] o[ "; // outgoing blocks now
  for (const auto &block : self.m_outgoing) os << '#' << fmt::format("{0}", fmt::ptr(block)) << ' ';

  os << "]\n"; // show the instructions now
  for (const auto &instruction : self.m_instructions) os << "  |   " << *instruction << '\n';
}
