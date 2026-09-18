#ifndef _SABRE_BYTECODE_COMPILER_HPP
#define _SABRE_BYTECODE_COMPILER_HPP

/// Sabre Includes
#include "sabre/bytecode/assembler.hpp"
#include "sabre/bytecode/loop.hpp"
#include "sabre/bytecode/optimizer.hpp"
#include "sabre/bytecode/trace.hpp"
#include "sabre/diagnostic/inspect.hpp"
#include "sabre/function/info.hpp"
#include "sabre/shape/service.hpp"

/// Syntax Includes
#include "sabre/syntax/declaration/preamble.hpp"
#include "sabre/syntax/literal/identifier.hpp"

namespace Sabre::Bytecode {

/// @brief Declaration Alias.
using Declaration = std::pair<Register::Slot, bool>;

/// @brief Bytecode Compiler.
class Compiler : public XI::Transient {
  //  TYPEDEFS  //

  /// @brief Allow disposables internal inspection.
  friend class Disposable;

  //  PROPERTIES  //

  /// @brief Current compilation request.
  Request *m_request = nullptr;

  /// @brief Underlying services container.
  XI::Container *m_services = nullptr;

  /// @brief The current stack-traces.
  std::stack<XLSP::Position> m_traces = {};

  /// @brief The current bytecode arena.
  $::Unique::Pointer<Image::Arena> m_arena = nullptr;

  /// @brief Current disposable instance.
  const Disposable *m_disposable = nullptr;

  /// @brief Associated variable captures.
  const Variable::Captures *m_captures = nullptr;

  /// @brief The bytecode assembler service.
  $::Unique::Pointer<Assembler> m_assembler = nullptr;

  /// @brief The bytecode optimizer service.
  $::Unique::Pointer<Optimizer> m_optimizer = nullptr;

  /// @brief Current compilation pipeline.
  $::Unique::Pointer<Queue> m_queue = $::Unique::New<Queue>();

  /// @brief The binder for blocks/jumps labels.
  $::Unique::Pointer<Binder> m_labels = $::Unique::New<Binder>(&m_request);

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a bytecode compiler.
  explicit Compiler();

  /**
   * @brief Constructs a bytecode compiler.
   * @param services                  Services container.
   */
  explicit Compiler(XI::Container *services);

  //  PUBLIC METHODS  //

  /// @brief Gets the available labels.
  inline constexpr Binder *labels() const noexcept { return m_labels.get(); }

  /// @brief Gets the compilation queue.
  inline constexpr const Queue *queue() const noexcept { return m_queue.get(); }

  /// @brief Gets the shapes service.
  inline constexpr Shape::Service *shapes() const noexcept { return *m_services; }

  /// @brief Gets the current arena resource.
  inline constexpr $::URI::View resource() const noexcept { return m_arena->resource; }

  /// @brief Gets the current compilation routine.
  inline constexpr Routine *routine() const noexcept { return m_request->routine(); }

  /// @brief Gets the internal allocator for registers.
  inline constexpr Allocator *registers() const noexcept { return m_request->registers(); }

  /// @brief Gets the captures for variables.
  inline constexpr const Variable::Captures *captures() const noexcept { return m_captures; }

  /// @brief Gets the variable scoping.
  inline constexpr Variable::Scope *variables() const noexcept { return m_request->variables(); }

  /// @brief Constructs a scoped loop.
  inline constexpr Loop loop() { return Loop(this); }

  /// @brief Constructs a variable scoping.
  inline constexpr auto scope() { return m_request->scope(); }

  /**
   * @brief Check if a block has disposable elements.
   * @param block                     Block to resolve.
   */
  Disposable disposable(const Syntax::Block *block);

  /**
   * @brief Handles tracing positions.
   * @param position              Position to trace.
   */
  inline constexpr Trace trace(const XLSP::Position &position) { return Trace(&m_traces, position); }

  /**
   * @brief Handles tracing nodes.
   * @param node                  Node to trace.
   */
  inline constexpr Trace trace(const Syntax::Node *node) {
    return node->is<Syntax::Lambda>() ? Trace() : trace(node->trivia()->range().start);
  }

  /**
   * @brief Constructs a constant value.
   * @param value                     Value to make constant.
   */
  Index constant(const Value::Any &value);

  /**
   * @brief Constructs a string intern.
   * @param buffer                    Buffer to intern.
   */
  Index string(const $::String::View &buffer);

  /**
   * @brief Constructs a string/symbol intern.
   * @param buffer                    Buffer to intern.
   */
  Index symbol(const $::String::View &buffer);

  /**
   * @brief Handles enqueuing functions for compilation.
   * @param function                  Function to compile.
   */
  Index enqueue(const Syntax::Lambda *function) const;
  Index enqueue(const Syntax::Class *prototype) const;

  /**
   * @brief Constructs an import path intern.
   * @param sink                      Destination register.
   * @param buffer                    Buffer to intern.
   */
  void import(const Register::Slot &sink, const $::String::View &buffer);

  /**
   * @brief Handles loading/storing variables.
   * @param name                      Name of variable.
   * @param extent                    Variable extent.
   */
  void load(const $::String::View &name, Variable::Extent extent, const Register::Slot &sink);
  void store(const $::String::View &name, Variable::Extent extent, const Register::Slot &value);

  /**
   * @brief Handles lowering the incoming node.
   * @param node                      Node to lower.
   * @param destination               Destination to use.
   */
  void lower(const Syntax::Node *node, const Register::Slot &destination = {});

  /**
   * @brief Handles emitting preamble information.
   * @param preamble                  Preamble node.
   * @param value                     Source of preamble.
   */
  void preamble(const Syntax::Preamble *preamble, const Register::Slot &value);

  /**
   * @brief Handles exposing variables to modules.
   * @param declaration               Declaration node.
   * @param value                     Source of variable.
   * @param constant                  Optional constant.
   */
  void expose(const Syntax::Declaration *declaration, const Register::Slot &value);
  void expose(const Syntax::Declaration *declaration, const Register::Slot &value, const Value::Any &constant);

  /**
   * @brief Handles compiling syntax trees.
   * @param syntax                    Syntax to compile.
   */
  $::Unique::Pointer<Image::Arena> process(const Syntax::Tree *syntax);

  /**
   * @brief Handles compiling match guards.
   * @param guard                     Guard to check.
   * @param value                     Value to check.
   */
  Label match(const std::vector<Syntax::Expression *> &guards, const Register::Slot &value);

  /**
   * @brief Handles declaring a variable.
   * @param variable                  Variable to declare.
   */
  Declaration declare(const Syntax::Identifier *variable);
  Declaration declare(const Syntax::Declaration *variable);

  /**
   * @brief Handles declaring a variable.
   * @param name                      Name of variable.
   * @param node                      Associated node.
   */
  Declaration declare(const $::String::View &name, const Syntax::Node *node);

  /**
   * @brief Handles returning with a value.
   * @param value                     Value to panic.
   */
  inline void returns(const Syntax::Expression *value) { lower(value, Register::Accumulator), m_returns(); }

  /**
   * @brief Handles panicking with a value.
   * @param value                     Value to panic.
   */
  inline void panic(const Syntax::Expression *value) { lower(value, Register::Accumulator), m_panic(); }

  /**
   * @brief Handles emitting panics.
   * @param code                      Diagnostic code.
   * @param args                      Format arguments.
   */
  template <class... As> inline void panic(Diagnostic::Code code, As &&...args) {
    panic(Diagnostic::Inspect::format(code, std::forward<As>(args)...));
  }

  /**
   * @brief Handles emitting panics.
   * @param code                      Diagnostic code.
   * @param args                      Format arguments.
   */
  template <class... As> inline void panic(const $::String::View &message, As &&...args) {
    auto formatted = fmt::format(fmt::runtime(message), std::forward<As>(args)...);
    emit<Glyph::STRING_MAKE>(Register::Accumulator, string(formatted)), m_panic();
  }

  /**
   * @brief Emits an instruction to the current block.
   * @param operands                  Operands to emit.
   */
  template <Glyph::Encoded G, class... As> inline constexpr void emit(As &&...operands) {
    m_emit($::Unique::New<Qualified<G>>(std::forward<As>(operands)...));
  }

  /**
   * @brief Handles plugging instructions.
   * @param destination           Destination register.
   * @param operands              Instruction operands.
   */
  template <Glyph::Encoded G, class... As>
  inline constexpr void plug(const Register::Slot &destination, As &&...operands) {
    if (!destination.nowhere()) emit<G>(destination, std::forward<As>(operands)...);
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Denotes if the runtime is being bundled.
  bool m_bundled() const noexcept;

  /**
   * @brief Handles emitting an instruction.
   * @param instruction               Instruction to emit.
   */
  void m_emit(Boxed &&instruction);

  /**
   * @brief Handles compiling a function request.
   * @param request                   Bytecode request.
   */
  void m_function(Request *request);

  /**
   * @brief Handles compiling a class extension.
   * @param super                     Callable expression.
   */
  void m_inherits(const Syntax::Call *super);

  /**
   * @brief Handles compiling match guards.
   * @param guard                     Guard to check.
   * @param success                   Success label.
   * @param value                     Value to check.
   */
  void m_match(const Syntax::Expression *guard, const Label &success, const Register::Slot &value);

  /**
   * @brief Handles propagating constant loads.
   * @param name                      Name of variable.
   * @param sink                      Sink to output to.
   */
  bool m_propagate(const $::String::View &name, const Register::Slot &sink);

  /**
   * @brief Handles compiling a function parameter.
   * @param parameter                 Parameter to compile.
   * @param spread                    Expected spread parameter.
   */
  bool m_parameter(const Syntax::Variable *parameter, const Syntax::Variable *spread = nullptr);

  /// @brief Handles internal returning from the accumulator.
  inline void m_returns() { emit<Glyph::EXEC_RETURN>(), emit<Glyph::JUMP_TO>(m_labels->returns()); }

  /// @brief Handles internal panicking from the accumulator.
  inline void m_panic() { emit<Glyph::EXEC_PANIC>(), emit<Glyph::JUMP_TO>(m_labels->returns()); }
};

} // namespace Sabre::Bytecode

#endif
