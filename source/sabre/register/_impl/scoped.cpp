/// Sabre Includes
#include "sabre/register/scoped.hpp"
#include "sabre/bytecode/allocator.hpp"

//  CONSTRUCTORS  //

Sabre::Register::Scoped::Scoped(Bytecode::Allocator *allocator) : Slot(allocator->allocate()), m_allocator(allocator) {}
Sabre::Register::Scoped::~Scoped() { m_allocator->m_release(*this); }
