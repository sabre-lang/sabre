#ifndef _SABRE_OBJECT_ENUM_HPP
#define _SABRE_OBJECT_ENUM_HPP

/// Sabre Includes
#include "sabre/member/reference.hpp"
#include "sabre/number/tagged.hpp"
#include "sabre/string/common.hpp"

namespace Sabre {

/// @brief Enumeration Variant.
struct Object::Variant {
  /// @brief Underlying variant name.
  String::Any name;

  /// @brief Aliased variant label.
  String::Any label;

  /// @brief The bound member reference.
  $::Unique::Pointer<Member::Reference> value;
};

/// @brief Enumeration Attributes.
template <> struct Object::Wrapper<Object::Enum> {
  //  PROPERTIES  //

  /// @brief Available enumeration variants.
  $::Map::Base<Value::Symbol, Variant> variants = {};

  //  CONSTRUCTORS  //

  /// @brief Constructs an empty enumeration.
  explicit Wrapper() = default;

  /**
   * @brief Constructs an enumeration.
   * @param variants              Variants initializer.
   */
  explicit Wrapper(std::vector<Variant> &&variants) : Wrapper(m_initialize(std::move(variants))) {}

  /**
   * @brief Constructs an enumeration.
   * @param variants              Enumeration variants.
   */
  explicit Wrapper($::Map::Base<Value::Symbol, Variant> &&variants) : variants(std::move(variants)) {}

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles initializing variants.
   * @param initializer           Initializer list.
   */
  static inline $::Map::Base<Value::Symbol, Variant> m_initialize(std::vector<Variant> &&initializer) {
    auto predicate = [](Variant &&variant) { return std::pair(variant.name.symbol(), std::move(variant)); };
    auto range = std::views::as_rvalue(initializer) | std::views::transform(predicate);
    return $::Map::Base<Value::Symbol, Variant>(range.begin(), range.end()); // and re-cast
  }
};

/// @brief Enumeration Interface.
struct Object::Enum : public Object::Mixin<Object::Enum> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  /// @brief Gets the total variants available.
  inline constexpr const auto &variants() const noexcept { return m_wrapper()->variants; }

  /**
   * @brief Allows looking up an enumeration variant.
   * @param symbol                Symbol to resolve.
   */
  inline constexpr const Variant *resolve(const Value::Symbol &symbol) const noexcept {
    auto &view = variants(); // prepare the view now to be used
    return view.contains(symbol) ? &view.at(symbol) : nullptr;
  }

  /**
   * @brief Allows looking up an enumeration variant.
   * @param ordinal               Ordinal to resolve.
   */
  inline constexpr const Variant *resolve(Number::Tagged ordinal) const noexcept {
    for (const auto &variant : variants() | std::views::values) {
      if (variant.value->reference() == ordinal) return &variant;
    }

    // failed to find a suitably variant
    return nullptr;
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handlers yielding enumerations safely.
   * @param self                          Enum instance.
   * @param yield                         Yield callback.
   */
  static void m_yield(const Enum &self, Globals::Each &yield);
};

} // namespace Sabre

#endif
