#ifndef _SABRE_TYPE_STRUCTURE_HPP
#define _SABRE_TYPE_STRUCTURE_HPP

/// Type Includes
#include "sabre/type/utility/algorithm.hpp"

namespace Sabre::Type {

/// @brief Structured Interface Type.
class Structure : public Mixin<Structure> {
  //  PROPERTIES  //

  /// @brief Structure field types.
  Algorithm m_fields = {};

  /// @brief Associated structure name.
  $::String::Buffer m_name = {};

  /// @brief Prepare a fallback typing.
  Erased m_fallback = nullptr;

  /// @brief Class operators handler.
  Apply m_operators = nullptr;

  /// @brief Underlying structure generics.
  std::vector<Erased> m_constraints = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty interface.
  explicit Structure() = default;

  /**
   * @brief Constructs a named empty interface.
   * @param name                      Name to bind.
   */
  explicit Structure(const $::String::View &name) : m_name(name) {}

  /**
   * @brief Constructs a record typing.
   * @param fields                    Interface fields.
   */
  explicit Structure(const Erased &fallback) : m_fallback(fallback) {}

  /**
   * @brief Constructs a structure typing.
   * @param fields                    Interface fields.
   */
  explicit Structure(const Algorithm &fields) : m_fields(fields) {}

  /**
   * @brief Constructs a record typing.
   * @param name                      Name to bind.
   * @param fields                    Interface fields.
   */
  explicit Structure(const $::String::View &name, const Erased &fallback) : m_name(name), m_fallback(fallback) {}

  /**
   * @brief Constructs a structure typing.
   * @param name                      Name to bind.
   * @param fields                    Interface fields.
   */
  explicit Structure(const $::String::View &name, const Algorithm &fields) : m_fields(fields), m_name(name) {}
  explicit Structure(const $::String::View &name, const $::Map::Record<Entity> &fields) :
      m_fields(fields), m_name(name) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the bound name of a structure.
  inline constexpr $::String::Buffer &name() noexcept { return m_name; }
  inline constexpr $::String::View name() const noexcept { return m_name; }

  /// @brief Gets the fields for a structure.
  inline constexpr Algorithm &fields() noexcept { return m_fields; }
  inline constexpr const Algorithm &fields() const noexcept { return m_fields; }

  /// @brief The available class operators.
  inline constexpr Apply &operators() noexcept { return m_operators; }
  inline constexpr const Apply &operators() const noexcept { return m_operators; }

  /// @brief Gets the instantiated generics for a structure.
  inline constexpr std::vector<Erased> &constraints() noexcept { return m_constraints; }
  inline constexpr const std::vector<Erased> &constraints() const noexcept { return m_constraints; }

  /// @brief Gets the instantiated generics for a structure.
  inline constexpr Erased &constraints(size_t index) noexcept { return m_constraints.at(index); }
  inline constexpr const Erased &constraints(size_t index) const noexcept { return m_constraints.at(index); }

  /// @brief Gets the fallback monotyping.
  inline constexpr const Erased &fallback() const noexcept { return m_fallback; }

  /// @brief Gets the associated truthiness of a structure.
  inline constexpr $::Unit::Ternary truthiness() const noexcept final { return true; }

  /// @brief Structures always validate to object instances.
  inline constexpr Shape::Underlying shape() const noexcept final { return Shape::Lookup<Object::Instance>(); }

  /**
   * @brief Handles looking up fields.
   * @param field                 Field to lookup.
   */
  inline constexpr Entity lookup(const $::String::View &field) const final {
    if (auto entity = m_fields.lookup(field, this); !entity.unset()) return entity;
    return m_fallback == nullptr ? Entity() : Entity(m_fallback); // get fallback
  }

  /**
   * @brief Handles transforming the type.
   * @param kind                  Operator kind.
   */
  inline constexpr Erased apply(Operator::Kind kind) const final { return apply(kind, nullptr); }
  inline constexpr Erased apply(Operator::Kind kind, const Erased &right) const final {
    auto result = m_operators == nullptr ? New::unset() : m_operators(this, kind, right);
    return result->is<Unset>() ? right ? Mixin::apply(kind, right) : Mixin::apply(kind) : result;
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles inferring class types.
   * @param constraints               Generic constraints.
   */
  Erased m_infer(Constraints *constraints) const final;

  /**
   * @brief Handles running a unification pass.
   * @param candidate                 Candidate to unify.
   * @param constraints               Generic parameter constraints.
   */
  bool m_unify(const Erased &candidate, Constraints *constraints) const final;

  /**
   * @brief Handles printing the type.
   * @param os                        Output stream.
   * @param self                      Structure instance.
   */
  static void m_print(std::ostream &os, const Structure &self);
};

} // namespace Sabre::Type

#endif
