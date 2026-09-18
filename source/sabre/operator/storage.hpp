#ifndef _SABRE_OPERATOR_STORAGE_HPP
#define _SABRE_OPERATOR_STORAGE_HPP

/// Sabre Includes
#include "sabre/function/native.hpp"
#include "sabre/member/property.hpp"
#include "sabre/operator/kind.hpp"
#include "sabre/runtime/isolate.hpp"

namespace Sabre::Operator {

/// @brief Allows transforming values via-operators.
template <class T, class... As> using Transform = Value::Any (*)(Runtime::Isolate *, const T &, As...);

/// @brief Operator Storage Container.
template <class T> class Storage {
  //  TYPEDEFS  //

  /// @brief Ensures that types are derived from values.
  static_assert(std::derived_from<T, Value::Any>);

  /// @brief Initializer fields typing.
  using Unary = std::vector<std::pair<Kind, Transform<T>>>;
  using Binary = std::vector<std::pair<Kind, Transform<T, const Value::Any &>>>;

  //  PROPERTIES  //

  /// @brief Allows accessing member properties.
  $::Map::Base<Kind, Member::Property> m_entries = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty storage instance.
  explicit constexpr Storage() = default;

  /**
   * @brief Constructs a storage instance.
   * @param unary                 Unary operators.
   */
  explicit constexpr Storage(const Unary &unary) : Storage(unary, {}) {}

  /**
   * @brief Constructs a storage instance.
   * @param binary                Binary operators.
   */
  explicit constexpr Storage(const Binary &binary) : Storage({}, binary) {}

  /**
   * @brief Constructs a storage instance.
   * @param unary                 Unary operators.
   * @param binary                Binary operators.
   */
  explicit constexpr Storage(const Unary &unary, const Binary &binary) : m_entries(unary.size() + binary.size()) {
    for (const auto &[kind, callback] : unary) m_entries.emplace(kind, m_factory(callback));
    for (const auto &[kind, callback] : binary) m_entries.emplace(kind, m_factory(callback));
  }

  //  PUBLIC METHODS  //

  /**
   * @brief Checks if a symbol exists.
   * @param kind                  Operator kind.
   */
  inline constexpr bool contains(Kind kind) const noexcept { return m_entries.contains(kind); }

  /**
   * @brief Handles looking up a storage field.
   * @param kind                  Operator kind.
   */
  inline constexpr Member::View lookup(Kind kind) noexcept {
    auto iter = m_entries.find(kind); // attempt resolving
    return iter == m_entries.cend() ? nullptr : &iter->second;
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles constructing properties.
   * @param transform             Unary transform.
   */
  inline constexpr Member::Property m_factory(Transform<T> unary) const noexcept {
    return Member::Property(Member::Getter([=](Runtime::Isolate *isolate, const Value::Any &self) {
      return unary(isolate, self.as<T>());
    }));
  }

  /**
   * @brief Handles constructing properties.
   * @param binary                Binary transform.
   */
  inline constexpr Member::Property m_factory(Transform<T, const Value::Any &> binary) const noexcept {
    return Member::Property(
        Member::Setter([=](Runtime::Isolate *isolate, const Value::Any &self, const Value::Any &right) {
          return binary(isolate, self.as<T>(), right);
        })
    );
  }
};

} // namespace Sabre::Operator

#endif
