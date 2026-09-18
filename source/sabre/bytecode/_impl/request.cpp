/// Sabre Modules
#include "sabre/bytecode/compiler.hpp"
#include "sabre/bytecode/routine.hpp"

//  CONSTRUCTORS  //

Sabre::Bytecode::Request::Request(const Syntax::Lambda *function, const $::Shared::Pointer<Variable::Scope> &upvalues) :
    Request(function->signature()->prototype(), function->body(), upvalues) {}

Sabre::Bytecode::Request::Request(const Syntax::Class *prototype, const $::Shared::Pointer<Variable::Scope> &upvalues) :
    Request(prototype->constructor(), prototype->block(), upvalues) {
  m_super = prototype->super(); // bind the super-call as well
}

Sabre::Bytecode::Request::Request(
    const Syntax::Constructor *signature, const Syntax::Node *body, const $::Shared::Pointer<Variable::Scope> &upvalues
) :
    m_body(body), m_signature(signature), m_registers($::Unique::New<Allocator>()),
    m_variables($::Shared::New<Variable::Scope>(upvalues)) {}

//  PUBLIC METHODS  //

$::Unique::Pointer<Sabre::Bytecode::Request> Sabre::Bytecode::Queue::dequeue() noexcept {
  auto top = std::move(m_requests.front());
  return m_requests.pop(), std::move(top);
}

Sabre::Bytecode::Index Sabre::Bytecode::Queue::enqueue(const Syntax::Lambda *function) {
  return enqueue(function, nullptr);
}

Sabre::Bytecode::Index Sabre::Bytecode::Queue::enqueue(const Syntax::Class *prototype) {
  return enqueue(prototype, nullptr);
}

Sabre::Bytecode::Index
Sabre::Bytecode::Queue::enqueue(const Syntax::Lambda *function, const $::Shared::Pointer<Variable::Scope> &upvalues) {
  return m_requests.emplace($::Unique::New<Request>(function, upvalues)), m_offset++;
}

Sabre::Bytecode::Index
Sabre::Bytecode::Queue::enqueue(const Syntax::Class *prototype, const $::Shared::Pointer<Variable::Scope> &upvalues) {
  return m_requests.emplace($::Unique::New<Request>(prototype, upvalues)), m_offset++;
}
