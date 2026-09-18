#ifndef _SABRE_OBJECT_EXCEPTION_HPP
#define _SABRE_OBJECT_EXCEPTION_HPP

/// Sabre Includes
#include "sabre/diagnostic/inspect.hpp"
#include "sabre/iterable/list.hpp"
#include "sabre/resource/trace.hpp"
#include "sabre/string/common.hpp"

namespace Sabre {

/// @brief Exception Attributes.
template <> struct Object::Wrapper<Object::Exception> {
  //  PROPERTIES  //

  String::Any name = {};    // Name of exception.
  String::Any message = {}; // Exception message.

  /// @brief Backtrace list.
  std::vector<Resource::Trace> trace = {};

  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a defaulted exception.
   * @param isolate                   Runtime isolate.
   */
  explicit Wrapper(Runtime::Isolate *isolate);

  /**
   * @brief Constructs an exception.
   * @param isolate                   Runtime isolate.
   * @param message                   Exception message.
   */
  explicit Wrapper(Runtime::Isolate *isolate, String::Any message);
  explicit Wrapper(Runtime::Isolate *isolate, const $::String::View &message);

  /**
   * @brief Constructs an exception.
   * @param isolate                   Runtime isolate.
   * @param name                      Name of exception.
   * @param message                   Exception message.
   */
  explicit Wrapper(Runtime::Isolate *isolate, String::Any name, String::Any message);
  explicit Wrapper(Runtime::Isolate *isolate, const $::String::View &name, const $::String::View &message);
};

/// @brief Exception Interface.
struct Object::Exception : public Object::Mixin<Object::Exception> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  inline constexpr String::Any &name() const noexcept { return m_wrapper()->name; }
  inline constexpr String::Any &message() const noexcept { return m_wrapper()->message; }
  inline constexpr std::vector<Resource::Trace> &trace() const noexcept { return m_wrapper()->trace; }
  inline constexpr std::ostream &format(std::ostream &os) const noexcept { return m_format(os); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles formatting exceptions.
   * @param os                     Output stream.
   */
  std::ostream &m_format(std::ostream &os) const noexcept;

  /**
   * @brief Handlers yielding exceptions safely.
   * @param self                  Exception instance.
   * @param yield                 Yield callback.
   */
  static void m_yield(const Exception &self, Globals::Each &yield);

  /**
   * @brief Handles printing values.
   * @param os                    Output stream.
   * @param self                  Exception instance.
   */
  static void m_print(std::ostream &os, const Exception &self);
};

} // namespace Sabre

#endif
