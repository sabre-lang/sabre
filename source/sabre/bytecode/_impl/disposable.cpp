/// Sabre Modules
#include "sabre/bytecode/disposable.hpp"
#include "sabre/bytecode/compiler.hpp"

//  CONSTRUCTORS  //

Sabre::Bytecode::Disposable::Disposable(Compiler *compiler) : Disposable(compiler, nullptr) {}
Sabre::Bytecode::Disposable::Disposable(Compiler *compiler, const Disposable *ancestor) :
    m_depth(ancestor ? ancestor->m_depth + 1 : 0), m_compiler(compiler), m_ancestor(ancestor) {
  m_compiler->m_disposable = this, m_compiler->emit<Glyph::DISPOSE_OPEN>(m_depth);
}

Sabre::Bytecode::Disposable::~Disposable() {
  if (m_compiler != nullptr) {
    m_compiler->m_disposable = m_ancestor;
    m_compiler->emit<Glyph::DISPOSE_CLOSE>(m_depth);
  }
}
