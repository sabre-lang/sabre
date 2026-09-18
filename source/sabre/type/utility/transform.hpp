#ifndef _SABRE_TYPE_TRANSFORM_HPP
#define _SABRE_TYPE_TRANSFORM_HPP

/// Sabre Includes
#include "sabre/type/entity.hpp"
#include "sabre/type/registry.hpp"

namespace Sabre::Type {

/// @brief Lazy Type Evaluation.
class Transform : public Mixin<Transform> {
  //  PROPERTIES  //

  /// @brief The baseline target.
  Erased m_target = New::any();

  /// @brief Bound resolvers available.
  Resolver m_resolver = [](const Erased &target, Constraints *) { return target; };

public:
  //  PUBLIC METHODS  //

  /**
   * @brief Constructs a lazy-transform.
   * @param target                Target to transform.
   */
  explicit Transform(const Erased &target = New::any()) : m_target(target) {}

  /**
   * @brief Constructs a transformation.
   * @param target                Target to transform.
   * @param resolver              Resolver to inherit.
   */
  explicit Transform(const Erased &target, Resolver &&resolver) : m_target(target), m_resolver(std::move(resolver)) {}

  //  PUBLIC METHODS  //

  /// @brief Allow getting the attached target.
  inline constexpr Erased &target() noexcept { return m_target; }
  inline constexpr const Erased &target() const noexcept { return m_target; }

  /// @brief Gets the associated truthiness of the target.
  inline constexpr $::Unit::Ternary truthiness() const noexcept final { return reduce()->truthiness(); }

  /// @brief Gets the shape of a generic.
  inline constexpr Shape::Underlying shape() const noexcept final { return reduce()->shape(); }

  /**
   * @brief Handles reducing a target against constraints.
   * @param constraints           Constraints to resolve.
   */
  inline constexpr Erased reduce(Constraints *constraints = nullptr) const { return m_resolver(m_target, constraints); }

  /**
   * @brief Handles looking up fields.
   * @param field                     Field to lookup.
   */
  inline Entity lookup(const $::String::View &field) const final { return reduce()->lookup(field); }

  /**
   * @brief Handles transforming the type.
   * @param kind                      Operator kind.
   */
  inline Erased apply(Operator::Kind kind) const final { return reduce()->apply(kind); }

  /**
   * @brief Handles transforming the type.
   * @param kind                      Operator kind.
   * @param right                     RHS value to use.
   */
  inline Erased apply(Operator::Kind kind, const Erased &right) const final { return reduce()->apply(kind, right); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles instantiating a transform.
   * @param constraints               Generic constraints.
   */
  inline Erased m_infer(Constraints *constraints) const final { return reduce(constraints)->m_infer(constraints); }

  /**
   * @brief Handles running a unification pass.
   * @param candidate                 Candidate to unify.
   * @param constraints               Generic constraints.
   */
  inline bool m_unify(const Erased &candidate, Constraints *constraints) const final {
    return reduce(constraints)->m_unify(candidate, constraints);
  }

  /**
   * @brief Handles printing the type.
   * @param os                        Output stream.
   */
  static inline void m_print(std::ostream &os, const Transform &self) { os << *self.reduce(); }
};

} // namespace Sabre::Type

#endif
