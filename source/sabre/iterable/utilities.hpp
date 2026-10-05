#ifndef _SABRE_ITERABLE_UTILITIES_HPP
#define _SABRE_ITERABLE_UTILITIES_HPP

/// Sabre Includes
#include "sabre/iterable/interval.hpp"
#include "sabre/iterable/list.hpp"

namespace Sabre::Iterable {

/// @brief Handles unpacking a value to a span.
static inline struct {
  //  OPERATOR METHODS  //

  /**
   * @brief Handles unpacking a value to a span.
   * @param value                 Value to unpack.
   */
  inline constexpr std::span<Value::Any> operator()(const Value::Any &value) const noexcept {
    return value.is<List>() ? value.as<List>().span() : std::span<Value::Any>();
  }
} Unpack;

/// @brief Helper for resolving iterator values.
static inline struct {
  //  OPERATOR METHODS  //

  /**
   * @brief Resolves iterator values.
   * @param isolate               Runtime isolate.
   * @param value                 Potential iterator.
   */
  inline constexpr Value::Any operator()(Runtime::Isolate *isolate, const Value::Any &value) const noexcept {
    // fast-path immediate iterator values here
    if (value.is<Iterator>()) return value;

    // otherwise attempt resolving via the attribute descriptor
    auto *descriptor = value.attribute(Operator::Kind::ITER);
    return descriptor ? descriptor->getter(isolate, value) : Value::Missing();
  }
} Resolve;

} // namespace Sabre::Iterable

namespace Sabre::Iterable::Deduce {
//  PUBLIC METHODS  //

/**
 * @brief Handles deducing interval values.
 * @param isolate                 Runtime isolate.
 * @param args                    Function arguments.
 */
std::optional<Interval> interval(Runtime::Isolate *isolate, const Function::Args &args);

/**
 * @brief Handles deducing slice values.
 * @param isolate                 Runtime isolate.
 * @param args                    Function arguments.
 * @param size                    Expected size value.
 */
std::optional<Slice> slice(Runtime::Isolate *isolate, const Function::Args &args, int64_t size);

} // namespace Sabre::Iterable::Deduce

#endif
