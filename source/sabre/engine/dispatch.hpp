#ifndef _SABRE_ENGINE_DISPATCH_HPP
#define _SABRE_ENGINE_DISPATCH_HPP

/// Sabre Includes
#include "sabre/function/frame.hpp"

namespace Sabre::Engine {

/// @brief Inheritance Subtyping Result.
enum class Subtype : uint8_t { SUCCESS, FAILURE, MISMATCH };

/// @brief Interpreter Dispatching.
class Dispatch : public $::Ensure::Static {
  //  TYPEDEFS  //

  /// @brief Prepare a set of glyph aliases.
  using Glyph = Bytecode::Glyph;

  /// @brief Prepare an instruction alias.
  using Instruction = Bytecode::Instruction;

  /// @brief Feedback Analysis Structure.
  struct Feedback {
    Value::Any target = Value::Void();
    Value::Any field = Value::Void();
    Value::Symbol symbol = Value::Symbol();
  };

public:
  //  PUBLIC METHODS  //

  /**
   * @brief Handles concatenating two values.
   * @param isolate                 Runtime isolate.
   * @param left                    Left-most value.
   * @param right                   Right-most value.
   */
  static Value::Any concat(Isolate *isolate, const Value::Any &left, const Value::Any &right);
  static Value::Any concat(Isolate *isolate, const String::Any &left, const String::Any &right);

  /**
   * @brief Handles dispatching a call operations.
   * @param isolate               Runtime isolate.
   * @param target                Target to call.
   * @param args                  Arguments to apply.
   */
  static Value::Any spawn(Isolate *isolate, const Value::Any &target, const Args &args = {});
  static Value::Any invoke(Isolate *isolate, const Value::Any &target, const Args &args = {});

  /**
   * @brief Handles setting an attribute.
   * @param isolate               Runtime isolate.
   * @param target                Target to call.
   * @param value                 Value to assign.
   * @param symbol                Symbol to set.
   */
  static Value::Any getter(Isolate *isolate, const Value::Any &target, const Value::Symbol &symbol);
  static Value::Any setter(Isolate *isolate, const Value::Any &target, const Value::Any &value, const Value::Symbol &);

  /**
   * @brief Handles override attributes.
   * @param isolate               Runtime isolate.
   * @param target                Target to bind.
   * @param callback              Callback to bind.
   * @param kind                  Operator kind.
   */
  static Value::Any overrides(Isolate *isolate, const Value::Any &target, const Value::Any &cb, Operator::Kind kind);

  /**
   * @brief Handles calling a super constructor.
   * @param isolate               Runtime isolate.
   * @param instance              Class instance.
   * @param args                  Arguments to supply.
   */
  static Value::Any super(Isolate *isolate, const Object::Instance &instance, const Args &args = {});

  /**
   * @brief Handles binding constructors.
   * @param isolate               Runtime isolate.
   * @param prototype             Class prototype.
   * @param info                  Function information.
   * @param context               Function environment.
   */
  static Value::Any constructor(Isolate *isolate, const Object::Class &, const Function::Info *, const Value::Any &);

  /**
   * @brief Handles calling a member constructor.
   * @param isolate               Runtime isolate.
   * @param instance              Class instance.
   * @param intern                Member name.
   * @param value                 Member value.
   * @param immutable             Immutable flag.
   */
  static Value::Any member(Isolate *, const Object::Instance &, const String::Intern *, const Value::Any &, bool);

  /**
   * @brief Handles constructing objects.
   * @param isolate               Runtime isolate.
   * @param args                  Arguments to apply.
   */
  static Value::Any object(Isolate *isolate, const Args &args = {});

  /**
   * @brief Constructs an iterator from an iterable.
   * @param isolate               Runtime isolate.
   * @param iterable              Iterable value.
   */
  static Value::Any iterator(Isolate *isolate, const Value::Any &iterable);

  /**
   * @brief Handles constructing enumerations.
   * @param isolate               Runtime isolate.
   * @param args                  Arguments to apply.
   */
  static Value::Any enumeration(Isolate *isolate, const Args &args = {});

  /**
   * @brief Checks if a type is a valid match.
   * @param value                 Value to check.
   * @param guard                 Incoming guard.
   */
  static bool matches(const Value::Any &value, const Value::Any &guard);

  /**
   * @brief Checks if a type extends another.
   * @param value                 Value to check.
   * @param guard                 Incoming guard.
   */
  static Subtype extends(const Value::Any &value, const Value::Any &guard);

  /**
   * @brief Ensures if a type extends another.
   * @param value                 Value to check.
   * @param guard                 Incoming guard.
   */
  static Value::Any ensure(Isolate *isolate, const Value::Any &value, const Value::Any &guard);

  /**
   * @brief Handles exposing barrel exports.
   * @param isolate               Runtime isolate.
   * @param frame                 Engine frame.
   * @param object                Barrel value.
   */
  static Value::Any barrel(Isolate *isolate, const Frame *frame, const Object::Instance &object);

  /**
   * @brief Handles exposing an export value.
   * @param isolate               Runtime isolate.
   * @param frame                 Engine frame.
   * @param value                 Value to export.
   * @param intern                Given export name.
   */
  static Value::Any expose(Isolate *isolate, const Frame *frame, const Value::Any &value, const String::Intern *intern);

  /**
   * @brief Handles executing a closure frame.
   * @param isolate                 Runtime isolate.
   * @param frame                   Function frame.
   */
  $_WILLTAIL static Value::Any tailcall(Isolate *isolate, Function::Frame *frame, const Instruction * = nullptr);

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Forces an interrupt to occur.
   * @param isolate                 Runtime isolate.
   * @param frame                   Frame to interrupt.
   */
  static Value::Any m_interrupt(Isolate *isolate, Function::Frame *frame);

  /**
   * @brief Attempts jumping to the required index.
   * @param frame                   Function frame.
   * @param index                   Index to jump.
   */
  static bool m_jump(Function::Frame *frame, const Bytecode::Index &index);

  /**
   * @brief Handles constructing objects.
   * @param isolate               Runtime isolate.
   * @param args                  Arguments to apply.
   */
  static Value::Any m_object(Isolate *isolate, const std::span<Value::Any> &pairs = {});

  /**
   * @brief Handles constructing enumerations.
   * @param isolate               Runtime isolate.
   * @param args                  Arguments to apply.
   */
  static Value::Any m_enumeration(Isolate *isolate, const std::span<Value::Any> &tuples = {});

  /**
   * @brief Handles validating feedback.
   * @param isolate               Runtime isolate.
   * @param feedback              Feedback details.
   */
  static Value::Any m_feedback(Isolate *isolate, const Feedback &feedback);

  /**
   * @brief Handles dispatching a call operations.
   * @param isolate               Runtime isolate.
   * @param target                Target to call.
   * @param args                  Arguments to apply.
   */
  static Value::Any m_invoke(Isolate *isolate, const Value::Symbol &symbol, const Args &args = {});
  static Value::Any m_spawn(Isolate *isolate, const Value::Symbol &symbol, const Args &args = {});

  /**
   * @brief Handles exposing an export value.
   * @param isolate               Runtime isolate.
   * @param value                 Value to export.
   * @param intern                Given export name.
   */
  static Value::Any m_expose(Isolate *isolate, Exports *exports, const Value::Any &, const String::Intern *);
  static Value::Any m_expose(Isolate *isolate, const Object::Instance &, const Value::Any &, const String::Intern *);

  /**
   * @brief Handles direct matches.
   * @param value                 Value to check.
   * @param guard                 Guard to use.
   */
  static Subtype m_matches(const Value::Any &value, const Object::Class &guard);
  template <bool S> static Subtype m_matches(const Value::Any &value, const Value::Any &guard);

  /**
   * @brief Handles executing a singular instruction.
   * @param isolate               Runtime isolate.
   * @param frame                 Function frame.
   * @param instruction           Bytecode instruction.
   */
  template <Glyph::Encoded G>
  $_WILLTAIL static Value::Any m_execute(Isolate *isolate, Function::Frame *frame, const Instruction *instruction);
};

} // namespace Sabre::Engine

#endif
