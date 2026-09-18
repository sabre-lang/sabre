#ifndef _SABRE_BYTECODE_REQUEST_HPP
#define _SABRE_BYTECODE_REQUEST_HPP

/// Sabre Includes
#include "sabre/bytecode/routine.hpp"
#include "sabre/variable/scope.hpp"

/// Syntax Includes
#include "sabre/syntax/declaration/class.hpp"

namespace Sabre::Bytecode {

/// @brief Routine Request.
class Request {
  //  TYPEDEFS  //

  /// @brief Allow the compiler internal access.
  friend class Compiler;

  //  PROPERTIES  //

  /// @brief Bound function body.
  const Syntax::Node *m_body = nullptr;

  /// @brief The super constructor details.
  const Syntax::Call *m_super = nullptr;

  /// @brief Bound function signature.
  const Syntax::Constructor *m_signature = nullptr;

  /// @brief Routine to compile to.
  $::Unique::Pointer<Routine> m_routine = nullptr;

  /// @brief Allocator for underlying registers.
  $::Unique::Pointer<Allocator> m_registers = nullptr;

  /// @brief Variables reference.
  $::Shared::Pointer<Variable::Scope> m_variables = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a compilation request.
   * @param function              Function to compile.
   * @param upvalues              Optional upvalues.
   */
  explicit Request(const Syntax::Lambda *function, const $::Shared::Pointer<Variable::Scope> &upvalues = nullptr);
  explicit Request(const Syntax::Class *prototype, const $::Shared::Pointer<Variable::Scope> &upvalues = nullptr);

  /**
   * @brief Constructs a compilation request.
   * @param constructor           Constructor details.
   * @param body                  Function body.
   * @param upvalues              Optional upvalues.
   */
  explicit Request(
      const Syntax::Constructor *constructor,
      const Syntax::Node *body,
      const $::Shared::Pointer<Variable::Scope> &upvalues = nullptr
  );

  //  PUBLIC METHODS  //

  inline constexpr Routine *routine() const noexcept { return m_routine.get(); }
  inline constexpr Allocator *registers() const noexcept { return m_registers.get(); }
  inline constexpr Variable::Scope *variables() const noexcept { return m_variables.get(); }

  /// @brief Scopes a new set of variables.
  inline constexpr auto scope() {
    m_variables = $::Shared::New<Variable::Scope>(m_variables); // scope now
    return $::Lambda::Defer([&] { m_variables = m_variables->ancestor(); });
  }
};

/// @brief Routine Compilation Queue.
class Queue {
  //  PROPERTIES  //

  /// @brief Current compilation offset.
  Index m_offset = 0;

  /// @brief Currently queued compilation requests.
  std::queue<$::Unique::Pointer<Request>> m_requests = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a compilation queue.
  explicit Queue() = default;

  //  PUBLIC METHODS  //

  /// @brief Denotes if the queue is currently empty.
  inline constexpr bool empty() const noexcept { return m_requests.empty(); }

  /// @brief Gets the current size of the queue.
  inline constexpr size_t size() const noexcept { return m_requests.size(); }

  /**
   * @brief Handles enqueing functions.
   * @param function                  Function to enqueue.
   * @param upvalues                  Upvalues reference.
   */
  Index enqueue(const Syntax::Lambda *function);
  Index enqueue(const Syntax::Class *prototype);
  Index enqueue(const Syntax::Lambda *function, const $::Shared::Pointer<Variable::Scope> &upvalues);
  Index enqueue(const Syntax::Class *prototype, const $::Shared::Pointer<Variable::Scope> &upvalues);

  /// @brief Pops the front queue entry.
  $::Unique::Pointer<Request> dequeue() noexcept;
};

} // namespace Sabre::Bytecode

#endif
