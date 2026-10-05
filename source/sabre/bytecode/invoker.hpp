#ifndef _SABRE_BYTECODE_INVOKER_HPP
#define _SABRE_BYTECODE_INVOKER_HPP

/// Sabre Includes
#include "sabre/bytecode/allocator.hpp"
#include "sabre/function/policy.hpp"

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

  /// @brief Alias the internal policy details.
  using Policy = Function::Policy;

  /// @brief Internal arguments typing.
  using Args = std::vector<Syntax::Expression *>;

  /// @brief Potential Classification Typing.
  using Details = std::pair<Convention, const Syntax::Expression *>;

  //  PROPERTIES  //

  /// @brief Denotes if asynchronous.
  Policy m_policy = Policy::CALL;

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
   * @param policy            Execution policy given.
   */
  explicit Invoker(const Syntax::Expression *callee) : Invoker(m_classify(callee)) {}
  explicit Invoker(const Details &classification) : Invoker(classification, Policy::CALL) {}
  explicit Invoker(const Syntax::Expression *callee, Policy policy) : Invoker(m_classify(callee), policy) {}
  explicit Invoker(const Details &classification, Policy policy) :
      m_policy(policy), m_convention(classification.first), m_callee(classification.second) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the baseline calling convention.
  inline constexpr Convention convention() const noexcept { return m_convention; }

  /// @brief Resolves to the callee value.
  inline constexpr const Syntax::Expression *callee() const noexcept { return m_callee; }

  /// @brief Denotes if the invocation is inlinable.
  inline constexpr bool inlined() const noexcept {
    return m_convention == Convention::INLINE && m_policy == Policy::CALL;
  }

  /**
   * @brief Handles compiling the invocation.
   * @param compiler          Bytecode compiler.
   * @param destination       Destination register.
   * @param args              Call arguments.
   */
  void compile(Compiler *compiler, Register::Slot &destination, const Args &args = {}) const;

  /**
   * @brief Handles preparing an invocation.
   * @param compiler          Bytecode compiler.
   * @param destination       Destination register.
   * @param arguments         Call arguments.
   * @param async             Asynchronous flag.
   */
  Register::List prepare(Compiler *compiler, Register::Slot &destination, const Args &args = {}) const;

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles emitting invocations.
   * @param compiler          Byecode compiler.
   * @param destination       Destination register.
   * @param span              Optional arguments span.
   */
  template <Policy P> void m_bind(Compiler *compiler, const Register::Slot &) const noexcept;
  template <Policy P> void m_bind(Compiler *compiler, const Register::Slot &, const Register::Span &) const noexcept;

  /**
   * @brief Handles emitting invocations.
   * @param compiler          Byecode compiler.
   * @param destination       Destination register.
   * @param span              Optional arguments span.
   */
  void m_dispatch(Compiler *compiler, const Register::Slot &destination, const Register::Span &span) const noexcept;

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

  //  SPECIALIZATIONS  //

#define X(P, ...)                                                                                                \
  template <> void m_bind<Policy::P>(Compiler *, const Register::Slot &) const noexcept;                         \
  template <> void m_bind<Policy::P>(Compiler *, const Register::Slot &, const Register::Span &) const noexcept;
  SABRE_XX_FUNCTION_POLICIES(X)
#undef X
};

} // namespace Sabre::Bytecode

#endif
