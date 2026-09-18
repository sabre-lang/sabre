#ifndef _SABRE_TYPE_PROTOTYPE_HPP
#define _SABRE_TYPE_PROTOTYPE_HPP

/// Type Includes
#include "sabre/type/compound/callable.hpp"
#include "sabre/type/compound/structure.hpp"
#include "sabre/type/utility/union.hpp"

namespace Sabre::Type {

/// @brief Constructor Applicator Callback.
using Constructor = $::Shared::Functor<Erased(const Prototype *) const>;

/// @brief Type Class Node.
class Prototype : public Mixin<Prototype> {
  //  TYPEDEFS  //

  /// @brief Allow instances internal access.
  friend class Instance;

  //  PROPERTIES  //

  /// @brief Underlying shape.
  Shape::Underlying m_shape;

  /// @brief Super-class typing to inherit.
  Erased m_super = New::none();

  /// @brief The associated structure value.
  $::Shared::Pointer<Structure> m_structure;

  /// @brief The underlying base constructor.
  Constructor m_constructor = nullptr;

  /// @brief Static field properties available.
  $::Map::Record<Entity> m_statics = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a class instance.
   * @param name                      Name of class.
   * @param shape                     Underlying shape.
   * @param fields                    Initial fields.
   */
  explicit Prototype(const $::String::View &name, Shape::Underlying shape, const Algorithm &fields = {}) :
      m_shape(shape), m_structure($::Shared::New<Structure>(name, fields)) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the shape of the prototype.
  inline constexpr Shape::Underlying shape() const noexcept final { return Shape::Lookup<Object::Class>(); }

  /// @brief Gets the name of the prototype.
  inline constexpr $::String::View name() const noexcept { return m_structure->name(); }

  /// @brief Gets the associated truthiness of the prototype.
  inline constexpr $::Unit::Ternary truthiness() const noexcept final { return true; }

  /// @brief Gets the super typing.
  inline constexpr Erased &super() noexcept { return m_super; }
  inline constexpr const Erased &super() const noexcept { return m_super; }

  /// @brief The available class fields.
  inline constexpr Algorithm &fields() noexcept { return m_structure->fields(); }
  inline constexpr const Algorithm &fields() const noexcept { return m_structure->fields(); }

  /// @brief The available metaclass fields.
  inline constexpr $::Map::Record<Entity> &statics() noexcept { return m_statics; }
  inline constexpr const $::Map::Record<Entity> &statics() const noexcept { return m_statics; }

  /// @brief The available class operators.
  inline constexpr Apply &operators() noexcept { return m_structure->operators(); }
  inline constexpr const Apply &operators() const noexcept { return m_structure->operators(); }

  /// @brief The available class constructor.
  inline constexpr Constructor &constructor() noexcept { return m_constructor; }
  inline constexpr const Constructor &constructor() const noexcept { return m_constructor; }

  /// @brief The available class generics.
  inline constexpr std::vector<Erased> &constraints() noexcept { return m_structure->constraints(); }
  inline constexpr const std::vector<Erased> &constraints() const noexcept { return m_structure->constraints(); }

  /// @brief Handles instantiating the class instance.
  inline constexpr $::Shared::Pointer<Instance> instantiate() const { return m_instantiate(); }

  /// @brief The baseline handler for constructing prototypes.
  inline constexpr Erased callable() const noexcept { return m_constructor ? m_constructor(this) : New::any(); }

  /**
   * @brief Handles looking up static fields.
   * @param field                     Field to look up.
   */
  inline Entity lookup(const $::String::View &field) const final {
    if (m_statics.contains(field)) return m_statics.at(field);
    if (!m_super->is<Prototype>()) return Entity();
    return m_super->as<Prototype>()->lookup(field);
  }

protected:
  //  PRIVATE METHODS  //

  /// @brief Gets the base shape value.
  inline constexpr Shape::Underlying m_base() const noexcept {
    return m_super->is<Prototype>() ? m_super->as<Prototype>()->m_base() : m_shape;
  }

  /**
   * @brief Checks if this class contains the given shape.
   * @param shape                     Shape to check.
   */
  inline constexpr bool m_extends(Shape::Underlying shape) const noexcept {
    if (m_shape == shape) return true;
    if (!m_super->is<Prototype>()) return false;
    return m_super->as<Prototype>()->m_extends(shape);
  }

  /**
   * @brief Handles looking up member fields.
   * @param field                     Field to look up.
   */
  inline Entity m_lookup(const $::String::View &field) const {
    // attempt getting the baseline entity
    auto entity = m_structure->lookup(field);
    if (!entity.unset()) return entity;

    // otherwise check against the super handler now
    if (!m_super->is<Prototype>()) return m_super->lookup(field);
    return m_super->as<Prototype>()->m_lookup(field);
  }

  /// @brief Handles constructing a class instantiation.
  $::Shared::Pointer<Instance> m_instantiate() const;

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
   * @param self                      Prototype instance.
   */
  static void m_print(std::ostream &os, const Prototype &self);
};

} // namespace Sabre::Type

#endif
