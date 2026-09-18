/// Sabre Modules
#include "sabre/bytecode/compiler.hpp"
#include "sabre/bundle/service.hpp"
#include "sabre/bytecode/disposable.hpp"
#include "sabre/bytecode/visitor.hpp"
#include "sabre/import/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/syntax/tree.hpp"
#include "sabre/value/fold.hpp"

//  CONSTRUCTORS  //

Sabre::Bytecode::Compiler::Compiler() : Compiler($::Global::get<Runtime::Container>()) {}
Sabre::Bytecode::Compiler::Compiler(XI::Container *services) : m_services(services) {
  m_optimizer = services->get<Optimizer>(m_labels.get());
  m_assembler = services->get<Assembler>(); // prebuild
}

//  PUBLIC METHODS  //

Sabre::Bytecode::Index Sabre::Bytecode::Compiler::constant(const Value::Any &value) {
  auto &constants = m_arena->constants;
  auto begin = constants.cbegin(), end = constants.cend();
  auto iter = std::ranges::find(constants, value);
  if (iter != end) return std::ranges::distance(begin, iter);
  return constants.emplace_back(value), constants.size() - 1;
}

Sabre::Bytecode::Index Sabre::Bytecode::Compiler::string(const $::String::View &raw) {
  // make sure to unescape the incoming string now
  auto buffer = $::Serde::Unescape(raw).value_or($::String::Buffer(raw));

  // prepare our details to be used
  auto &table = m_arena->strings;
  auto symbol = Value::Symbol(buffer);
  auto begin = table.cbegin(), end = table.cend();

  // attempt finding a valid result now
  for (auto iter = begin; iter != end; ++iter) {
    if (iter->symbol() == symbol) return std::distance(begin, iter);
  }

  // failed to find, so we want to emplace a value now
  return table.emplace_back(String::Intern(buffer, symbol)), table.size() - 1;
}

Sabre::Bytecode::Index Sabre::Bytecode::Compiler::symbol(const $::String::View &buffer) {
  return constant(m_arena->strings.at(string(buffer)).symbol());
}

void Sabre::Bytecode::Compiler::import(const Register::Slot &sink, const $::String::View &buffer) {
  // do a regular import if we are dynamically running out program
  if (!m_bundled()) return emit<Glyph::MODULE_IMPORT>(sink, string(buffer));

  // otherwise we want to resolve the resource now
  auto result = m_services->get<Import::Service>()->resolve(buffer, resource().body());

  // if this failed, then declare a bytecode error now
  if (!result.has_value()) return panic(8000000, result.error());

  // otherwise check if the resource is file-based or not
  if (result->scheme() != $::URI::Scheme::FILE) return emit<Glyph::MODULE_IMPORT>(sink, string(buffer));

  // since a valid file, then we should emit the import as an archive
  auto resource = $::URI::Buffer(Resource::Scheme::ARCH, result->relative().string());
  emit<Glyph::MODULE_IMPORT>(sink, string(resource.view()));
}

void Sabre::Bytecode::Compiler::lower(const Syntax::Node *node, const Register::Slot &sink) {
  Visitor::visit(node, this, Register::Slot(sink));
}

Sabre::Bytecode::Disposable Sabre::Bytecode::Compiler::disposable(const Syntax::Block *block) {
  // count the total resources that will be disposed on this blocks completion
  auto resources = std::ranges::count_if(block->statements(), [](const Syntax::Node *node) -> bool {
    if (!node->is<Syntax::Variable>()) return false;
    return node->as<Syntax::Variable>()->disposable();
  });

  // construct the resulting stack based on the total resources
  return resources ? Disposable(this, m_disposable) : Disposable();
}

Sabre::Bytecode::Label
Sabre::Bytecode::Compiler::match(const std::vector<Syntax::Expression *> &guards, const Register::Slot &value) {
  // reserve the base label to be used
  auto success = m_labels->reserve();

  // iterate over the available query guards to match
  for (const auto &guard : guards) m_match(guard, success, value);

  // return the resulting label now
  return success;
}

Sabre::Bytecode::Declaration Sabre::Bytecode::Compiler::declare(const Syntax::Identifier *node) {
  return declare(node->name(), node);
}

Sabre::Bytecode::Declaration Sabre::Bytecode::Compiler::declare(const Syntax::Declaration *node) {
  return declare(node->name(), node);
}

Sabre::Bytecode::Declaration Sabre::Bytecode::Compiler::declare(const $::String::View &name, const Syntax::Node *node) {
  auto leaked = m_captures->extent(node) == Variable::Extent::LEAKED;
  auto vreg = leaked ? Register::Slot(routine()->shared->leaked++) : registers()->allocate();
  return variables()->declare(name, vreg, leaked), Declaration(vreg, leaked);
}

void Sabre::Bytecode::Compiler::preamble(const Syntax::Preamble *node, const Register::Slot &source) {
  // update our value with the available decorators/attributes
  for (const auto &attribute : node->attributes()) lower(attribute, source);
  for (const auto &decorator : node->decorators()) lower(decorator, source);

  // and ensure we expose the value as necessary now
  expose(node, source);
}

void Sabre::Bytecode::Compiler::expose(const Syntax::Declaration *node, const Register::Slot &sink) {
  expose(node, sink, Value::Failure());
}

void Sabre::Bytecode::Compiler::expose(
    const Syntax::Declaration *node, const Register::Slot &sink, const Value::Any &value
) {
  // get the underlying modifiers to be handled
  const auto &modifiers = node->modifiers();

  // check for current immutability as well
  auto immutable = modifiers.test(Variable::Flag::MUTABLE);

  // check for exports that need to be exposed
  auto exports = modifiers.test(Variable::Flag::EXPORT);

  // check for object fields to be exposed as well
  auto fields = modifiers.test(Variable::Flag::PUBLIC, Variable::Flag::PRIVATE, Variable::Flag::PROTECTED);

  // if we have a suitable value and not doing either of the next two items
  auto preload = value.pointer().okay() && (fields || exports);
  if (preload) emit<Glyph::LOAD_CONST>(sink, constant(value));

  // handle exports to be shown
  if (exports) emit<Glyph::MODULE_EXPORT>(sink, string(node->name()));
  if (fields) emit<Glyph::CLASS_EXPORT>(sink, string(node->name()), immutable);
}

Sabre::Bytecode::Index Sabre::Bytecode::Compiler::enqueue(const Syntax::Lambda *function) const {
  return m_queue->enqueue(function, m_request ? m_request->m_variables : nullptr);
}

Sabre::Bytecode::Index Sabre::Bytecode::Compiler::enqueue(const Syntax::Class *prototype) const {
  return m_queue->enqueue(prototype, m_request ? m_request->m_variables : nullptr);
}

void Sabre::Bytecode::Compiler::load(const $::String::View &name, Variable::Extent extent, const Register::Slot &sink) {
  // ignore if there is no valid output destination
  if (sink.nowhere()) return;

  // pre-check against constant propagation
  if (m_propagate(name, sink)) return;

  // handle based on the incoming extent value
  switch (extent) {
  case Variable::Extent::LOCAL: {
    auto leaked = variables()->leaked(name);
    auto vreg = variables()->resolve(name);

    if (leaked) emit<Glyph::LOAD_CONTEXT>(sink, vreg);
    else if (vreg != sink) emit<Glyph::REG_MOVE>(sink, vreg);
  } break;

  case Variable::Extent::UPVALUE: {
    auto upvalue = variables()->upvalue(name);
    $_ASSERT(!upvalue.slot.nowhere(), "Invalid upvalue lookup");
    if (!upvalue.depth) emit<Glyph::LOAD_CONTEXT>(sink, upvalue.slot);
    else emit<Glyph::LOAD_UPVALUE>(sink, upvalue.slot, upvalue.depth);
  } break;

  // these are more simply loaded items
  case Variable::Extent::GLOBAL: emit<Glyph::LOAD_GLOBAL>(sink, constant(Value::Symbol(name))); break;

  // ensure that our extent is invalid here since unknown
  default: $_ABORT("Cannot load leaked variables");
  }
}

void Sabre::Bytecode::Compiler::store(
    const $::String::View &name, Variable::Extent extent, const Register::Slot &value
) {
  // prepare the source register to be used now
  auto slot = value.nowhere() ? Register::Accumulator : value;

  // check if we should fail on constant loads (eg: cannot normally assign)
  if (variables()->constant(name).pointer().okay()) return panic(3000100, name);

  // handle based on the incoming extent
  switch (extent) {
  case Variable::Extent::LOCAL: {
    auto leaked = variables()->leaked(name);
    auto vreg = variables()->resolve(name);

    if (leaked) emit<Glyph::STORE_CONTEXT>(vreg, slot);
    else if (vreg != slot) emit<Glyph::REG_MOVE>(vreg, slot);
  } break;

  case Variable::Extent::UPVALUE: {
    auto upvalue = variables()->upvalue(name);
    $_ASSERT(!upvalue.slot.nowhere(), "Invalid upvalue lookup");
    if (!upvalue.depth) emit<Glyph::STORE_CONTEXT>(upvalue.slot, slot);
    else emit<Glyph::STORE_UPVALUE>(upvalue.slot, slot, upvalue.depth);
  } break;

  case Variable::Extent::GLOBAL: panic("Cannot modify global variable '{0}'", name); break;
  default: $_ABORT("Cannot load leaked variables"); // ensure that our extent is valid here
  }
}

$::Unique::Pointer<Sabre::Image::Arena> Sabre::Bytecode::Compiler::process(const Syntax::Tree *syntax) {
  // prepare the base-line details to be used for compilation
  m_captures = syntax->captures();
  m_arena = $::Unique::New<Image::Arena>();

  // attempt lowering from our syntax-tree
  lower(syntax);

  // update the arenas associated resource
  m_arena->resource = syntax->resource();

  // process the incoming routine queue now
  while (m_queue->size()) {
    auto request = m_queue->dequeue();
    m_function(m_request = request.get());
  }

  // and return the final result after cleanup
  return m_captures = nullptr, m_request = nullptr, std::move(m_arena);
}

//  PRIVATE METHODS  //

bool Sabre::Bytecode::Compiler::m_bundled() const noexcept { return m_services->exists<Bundle::Service>(); }

bool Sabre::Bytecode::Compiler::m_propagate(const $::String::View &name, const Register::Slot &sink) {
  auto value = variables()->constant(name);
  if (!value.pointer().okay()) return false;
  return emit<Glyph::LOAD_CONST>(sink, constant(value)), true;
}

void Sabre::Bytecode::Compiler::m_emit(Boxed &&instruction) {
  // get the details to emit the instruction
  auto *block = m_labels->head();
  auto branches = instruction->branches();

  // always ensure a label for the first instruction
  if (block->empty()) m_labels->patch();

  // push the instruction onto the block now and onto the source-mapping
  auto *reference = block->instructions().emplace_back(std::move(instruction)).get();
  if (m_traces.size()) m_assembler->m_positions.set(reference, m_traces.top());

  // construct a new block if the instruction branches
  if (branches) m_labels->scope();
}

void Sabre::Bytecode::Compiler::m_function(Request *request) {
  // construct a new shared-function to be used now
  auto *info = m_arena->functions.emplace_back($::Unique::New<Function::Info>(m_arena.get())).get();

  // pull out some of the request details to be used
  const auto *function = request->m_signature;

  // pull out the spread value as well to be compared
  const auto *spread = function->spread();

  // update the underlying shared information
  info->shared()->arity = function->arity();
  info->shared()->adicity = function->adicity();

  // emplace the information onto the request now
  request->m_routine = $::Unique::New<Routine>(info->shared());

  // prepare a new labels scoping to be used
  m_labels->clear(), m_labels->scope();

  // pull out the necessary parameters now
  auto parameters = function->parameters() ? function->parameters()->list() : std::vector<Syntax::Variable *>();

  // emit all the necessary parameters now for the function
  for (const auto &[ii, parameter] : $::Ranges::Each(parameters)) {
    if (m_parameter(parameter, spread)) info->shared()->vargs = ii;
  }

  // check if the function-body is a block or not
  auto destination = request->m_body->is<Syntax::Block>() ? Register::Slot() : Register::Accumulator;

  // if we have an inheritance expression, then run that constructor
  m_inherits(request->m_super);

  // prepare the function body to be used now
  auto returns = m_labels->m_returns.emplace(m_labels->reserve());
  lower(request->m_body, destination), m_labels->m_returns.pop(), m_labels->patch(returns);

  // and emit an empty return handler just-in-case
  if (destination.nowhere()) emit<Glyph::LOAD_VOID>();
  emit<Glyph::EXEC_RETURN>(); // push a final return

  // attempt optimizing the function now as necessary
  m_optimizer->process(request->routine());

  // and update our final details for the routine now
  info->shared()->locals = registers()->maximum();

  // finally attempt assembling the resulting buffer to be used
  info->bytecode() = m_assembler->process(request->routine(), m_arena.get());
}

bool Sabre::Bytecode::Compiler::m_parameter(const Syntax::Variable *parameter, const Syntax::Variable *spread) {
  // prepare the parameter details to be handled
  auto [dest, leaked] = declare(parameter);
  auto *initializer = parameter->initializer();
  auto treg = leaked ? registers()->allocate() : dest;

  // update the initializer if necessary
  if (initializer) {
    // prepare the skip handler to be used
    auto skip = m_labels->reserve();

    // we test the initial register if assigned
    emit<Glyph::JUMP_FILLED>(skip, treg);
    lower(initializer, treg), m_labels->patch(skip);
  }

  // and move into a suitable position if necessary
  if (leaked) emit<Glyph::STORE_CONTEXT>(dest, treg);

  // declare if the spread is same as the parameter
  return parameter == spread;
}

void Sabre::Bytecode::Compiler::m_inherits(const Syntax::Call *super) {
  // if there is no super instance, then ignore
  if (super == nullptr) return;

  // prepare all the incoming arguments to be called
  auto list = registers()->list();

  // compile down all the incoming arguments
  for (const auto &argument : super->arguments()) lower(argument, list.grow());

  // after compiling the parent, we want to call the super-constructor
  emit<Glyph::CLASS_SUPER>(Register::Accumulator, list);
}

void Sabre::Bytecode::Compiler::m_match(
    const Syntax::Expression *guard, const Label &success, const Register::Slot &value
) {
  // handle based on the incoming guard type
  switch (guard->trivia()->hash()) {
  // fallbacks should jump immediately to their condition
  case $::RTTI::Hash<Syntax::Fallback>(): return emit<Glyph::JUMP_TO>(success);

  // these values should match their respective constants
  case $::RTTI::Hash<Syntax::Void>(): return emit<Glyph::MATCH_VOID>(success, value);
  case $::RTTI::Hash<Syntax::True>(): return emit<Glyph::MATCH_TRUE>(success, value);
  case $::RTTI::Hash<Syntax::False>(): return emit<Glyph::MATCH_FALSE>(success, value);

  // strings need to resolve a suitable intern value
  case $::RTTI::Hash<Syntax::Text>(): {
    auto index = string(guard->as<Syntax::Text>()->buffer());
    return emit<Glyph::MATCH_TEXT>(success, value, index);
  }

  // numerics need to resolve a suitable constant value
  case $::RTTI::Hash<Syntax::Numeric>(): {
    auto numeric = guard->as<Syntax::Numeric>()->value();
    auto index = constant(Number::Tagged(numeric));
    return emit<Glyph::MATCH_CONST>(success, value, index);
  }

  // some items should instead be using their internal values for matching
  case $::RTTI::Hash<Syntax::Group>(): return m_match(guard->as<Syntax::Group>()->value(), success, value);
  case $::RTTI::Hash<Syntax::Typed>(): return m_match(guard->as<Syntax::Typed>()->value(), success, value);

  // operators should be resolved downwards if possible
  case $::RTTI::Hash<Syntax::Unary>(): $_FALLTHROUGH;
  case $::RTTI::Hash<Syntax::Binary>(): $_FALLTHROUGH;
  case $::RTTI::Hash<Syntax::Ternary>(): {
    // check if we have a suitable immediate value
    auto immediate = Value::Fold(guard);
    if (!immediate.pointer().okay()) break;

    // since valid, then handle as a constant instead
    return emit<Glyph::MATCH_CONST>(success, value, constant(immediate));
  }

  // break out for all default match cases to be compared
  default: break;
  }

  // for all other instances, we need to firstly lower the node
  lower(guard, Register::Accumulator);

  // then prepare a suitable match guard
  emit<Glyph::MATCH_GUARD>(success, value, Register::Accumulator);
}
