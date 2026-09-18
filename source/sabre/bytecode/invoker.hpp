#ifndef _SABRE_BYTECODE_INVOKER_HPP
#define _SABRE_BYTECODE_INVOKER_HPP

/// Sabre Modules
#include "sabre/bytecode/allocator.hpp"

/// Syntax Modules
#include "sabre/syntax/expression/accessor.hpp"
#include "sabre/syntax/expression/caret.hpp"
#include "sabre/syntax/expression/group.hpp"
#include "sabre/syntax/expression/typed.hpp"
#include "sabre/syntax/literal/identifier.hpp"

namespace Sabre::Bytecode {

/// @brief Available Call-Site Conventions
enum class Convention : uint8_t { VOID, INLINE, FIELD };

/// @brief Handles Call Invocations.
class Invoker {
  //  TYPEDEFS  //

  /// @brief Internal arguments typing.
  using Args = std::vector<Syntax::Expression *>;

  /// @brief Potential Classification Typing.
  using Details = std::pair<Convention, const Syntax::Expression *>;

  //  PROPERTIES  //

  /// @brief Denotes if asynchronous.
  bool m_async = false;

  /// @brief The convention to be used.
  Convention m_convention = Convention::VOID;

  /// @brief The associated callee to be used.
  const Syntax::Expression *m_callee = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a suitable call-invocation.
  explicit Invoker() = default;

  /**
   * @brief Constructs a expression based invocation.
   * @param callee            Callee to classify.
   * @param async             Whether asynchronous.
   */
  explicit Invoker(const Syntax::Expression *callee, bool async = false) : Invoker(m_classify(callee), async) {}
  explicit Invoker(const Details &classification, bool async = false) :
      m_async(async), m_convention(classification.first), m_callee(classification.second) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the baseline calling convention.
  inline constexpr Convention convention() const noexcept { return m_convention; }

  /// @brief Resolves to the callee value.
  inline constexpr const Syntax::Expression *callee() const noexcept { return m_callee; }

  /// @brief Denotes if the invocation is inlinable.
  inline constexpr bool inlined(bool async = false) const noexcept {
    return m_convention == Convention::INLINE && !async;
  }

  /**
   * @brief Handles compiling the invocation.
   * @param compiler          Bytecode compiler.
   * @param destination       Destination register.
   * @param args              Call arguments.
   */
  void compile(Compiler *compiler, Register::Slot destination, const Args &args = {}) const;

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles preparing an invocation.
   * @param compiler          Bytecode compiler.
   * @param destination       Destination register.
   * @param arguments         Call arguments.
   * @param async             Asynchronous flag.
   */
  Register::List m_prepare(Compiler *compiler, Register::Slot &destination, const Args &args = {}) const;

  /**
   * @brief Handles emitting synchronous invocation.
   * @param compiler          Byecode compiler.
   * @param destination       Destination register.
   * @param span              Optional arguments span.
   */
  void m_invoke(Compiler *compiler, const Register::Slot &destination) const noexcept;
  void m_invoke(Compiler *compiler, const Register::Slot &destination, const Register::Span &span) const noexcept;

  /**
   * @brief Handles emitting asynchronous invocations.
   * @param compiler          Byecode compiler.
   * @param destination       Destination register.
   * @param span              Optional arguments span.
   */
  void m_spawn(Compiler *compiler, const Register::Slot &destination) const noexcept;
  void m_spawn(Compiler *compiler, const Register::Slot &destination, const Register::Span &span) const noexcept;

  /**
   * @brief Handles classifying the invocation.
   * @param callee            Callee to classify.
   */
  static inline constexpr Details m_classify(const Syntax::Expression *callee) {
    switch (callee->trivia()->hash()) {
    case $::RTTI::Hash<Syntax::Caret>(): return {Convention::INLINE, callee};
    case $::RTTI::Hash<Syntax::Accessor>(): return {Convention::FIELD, callee};
    case $::RTTI::Hash<Syntax::Group>(): return m_classify(callee->as<Syntax::Group>()->value());
    case $::RTTI::Hash<Syntax::Typed>(): return m_classify(callee->as<Syntax::Typed>()->value());

    // stop for normal conventions to be found
    default: return {Convention::VOID, callee};
    }
  }
};

} // namespace Sabre::Bytecode

#endif
