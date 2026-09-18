/// Sabre Modules
#include "sabre/bytecode/compiler.hpp"

//  CONSTRUCTORS  //

Sabre::Bytecode::Loop::Loop(Compiler *compiler) : m_labels(compiler->labels()), m_compiler(compiler) {
  m_labels->patch(m_head = m_labels->reserve()), m_exit = m_labels->reserve();
  m_labels->m_breaks.push(m_exit), m_labels->m_continues.push(m_head);
}

Sabre::Bytecode::Loop::~Loop() {
  m_labels->m_breaks.pop(), m_labels->m_continues.pop(); // close now
  m_compiler->emit<Glyph::JUMP_TO>(m_head), m_labels->patch(m_exit);
}
