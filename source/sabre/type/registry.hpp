#ifndef _SABRE_TYPE_REGISTRY_HPP
#define _SABRE_TYPE_REGISTRY_HPP

/// Type Includes
#include "sabre/type/mixin.hpp"

namespace Sabre::Type {

/// Encapsulates Type Building/Registration.
struct New : public $::Ensure::Static {
  //  PUBLIC METHODS  //

  static const Erased &any();   // Intrinsic "Any" typing.
  static const Erased &none();  // Intrinsic "Void" typing.
  static const Erased &fail();  // Intrinsic "Poison" typing.
  static const Erased &unset(); // Intrinsic "Unset" typing.
  static const Erased &never(); // Intrinsic empty "Union" typing.

  static const Erased &number();  // Gets the number instance type.
  static const Erased &string();  // Gets the string instance type.
  static const Erased &symbol();  // Gets the symbol instance type.
  static const Erased &boolean(); // Gets the boolean instance type.

  /// @brief Gets the typed object instance.
  static const $::Shared::Pointer<Structure> &object();

  /// @brief Gets the typed exception instance.
  static const $::Shared::Pointer<Instance> &exception();

  /**
   * @brief Makes a type entity optional.
   * @param target                Target option.
   */
  static Entity optional(const Erased &target);

  /**
   * @brief Constructs a nullish typing.
   * @param target                Target to nullify.
   */
  static Erased maybe(const Erased &target);

  /**
   * @brief Constructs a typed list instance.
   * @param target                Target type parameter.
   */
  static Erased list(const Erased &target);

  /**
   * @brief Constructs a typed future instance.
   * @param target                Target type parameter.
   */
  static Erased future(const Erased &target);

  /**
   * @brief Constructs a typed iterator instance.
   * @param target                Target type parameter.
   */
  static Erased iterator(const Erased &target);

  /**
   * @brief Handles constructing record types.
   * @param value                 Value to bind.
   */
  static $::Shared::Pointer<Structure> record(const Erased &value);

  /**
   * @brief Handles constructing structural types.
   * @param fields                Interface fields.
   */
  static $::Shared::Pointer<Structure> interface();
  static $::Shared::Pointer<Structure> interface(const $::Map::Record<Entity> &fields);

  /**
   * @brief Handles constructing structural types.
   * @param name                  Name of interface.
   * @param fields                Interface fields.
   */
  static $::Shared::Pointer<Structure> interface(const $::String::View &name);
  static $::Shared::Pointer<Structure> interface(const $::String::View &name, const $::Map::Record<Entity> &fields);

  /**
   * @brief Constructs a typed result instance.
   * @param target                Target type parameter.
   * @param error                 Result error typing.
   */
  static Erased result(const Erased &target, const Erased &error = any());

  /**
   * @brief Constructs an enumeration typing.
   * @param name                  Name of enum.
   * @param shape                 Shape of enum.
   */
  static Erased enumeration(const $::String::View &name, Shape::Underlying shape);

  /**
   * @brief Constructs a class-prototype typing.
   * @param name                  Name of enum.
   * @param shape                 Shape of enum.
   */
  static $::Shared::Pointer<Prototype> prototype(const $::String::View &name, Shape::Underlying shape);

  /**
   * @brief Constructs a type-parameter.
   * @param name                  Name of parameter.
   * @param extends               Extension type.
   * @param fallback              Fallback type.
   */
  static $::Shared::Pointer<Parameter>
  constraint(const $::String::View &name, const Erased &extends = any(), const Erased &fallback = any());

  /**
   * @brief Constructs a generic type.
   * @param target                    Target to wrap.
   * @param parameters                Parameter types.
   */
  static $::Shared::Pointer<Generic> generic(const Erased &target, const Template &parameters);
  static $::Shared::Pointer<Generic> generic(const Erased &target, const $::Shared::Pointer<Parameter> &first);

  /**
   * @brief Constructs a lazy-transform type.
   * @param target                    Target to wrap.
   * @param resolver                  Resolver callback.
   */
  static $::Shared::Pointer<Transform> lazy(const Erased &target = any());

  /**
   * @brief Constructs a transform type.
   * @param target                    Target to wrap.
   * @param resolver                  Resolver callback.
   */
  static $::Shared::Pointer<Transform> transform(const Erased &target = any());
  static $::Shared::Pointer<Transform> transform(const Erased &target, Resolver &&resolver);

  /**
   * @brief Constructs various utility transforms.
   * @param target                    Target to wrap.
   */
  static $::Shared::Pointer<Transform> awaited(const Erased &target);
  static $::Shared::Pointer<Transform> asyncify(const Erased &target);

  /**
   * @brief Constructs the result of an iterable type.
   * @param target                    Target to wrap.
   * @param outer                     Wrap outside.
   */
  static $::Shared::Pointer<Transform> iterable(const Erased &target, bool outer = true);

  /**
   * @brief Constructs an invocation transform (for binding).
   * @param target                    Target to wrap.
   * @param async                     Asyncify callable.
   */
  static $::Shared::Pointer<Transform> invocation(const Erased &target, bool async = false);

  /**
   * @brief Constructs a callable typing.
   * @param returns               Return typing.
   * @param parameters            Parameter types.
   */
  static $::Shared::Pointer<Callable> function(const Erased &returns = any());
  static $::Shared::Pointer<Callable> function(const Erased &returns, const Entity &first);
  static $::Shared::Pointer<Callable> function(const Erased &returns, const std::vector<Entity> &parameters);
  static $::Shared::Pointer<Callable> function(const Erased &returns, const Entity &first, const Entity &second);

  /**
   * @brief Constructs a variadic callable typing.
   * @param returns               Return typing.
   * @param parameters            Parameter types.
   */
  static $::Shared::Pointer<Callable> variadic(const Erased &returns = any());
  static $::Shared::Pointer<Callable> variadic(const Erased &returns, const Entity &first);
  static $::Shared::Pointer<Callable> variadic(const Erased &returns, const std::vector<Entity> &parameters);
  static $::Shared::Pointer<Callable> variadic(const Erased &returns, const Entity &first, const Entity &second);

  /**
   * @brief Handles merging two types into a union.
   * @param left                  Left-most type.
   * @param right                 Right-most type.
   */
  static Erased merge(const Erased &left, const Erased &right);

  /**
   * @brief Handles coalescing nullish types.
   * @param left                  Maybe nullish type.
   * @param right                 Coalescing typing.
   */
  static Erased coalesce(const Erased &left, const Erased &right);

  /**
   * @brief Handles safe resolution casts.
   * @param type                  Type to resolve.
   * @param strict                Strictness flag.
   */
  template <class T> static inline constexpr $::Shared::Pointer<T> cast(const Erased &type, bool = true) {
    return type->is<T>() ? type->as<T>() : nullptr;
  }

  //  SPECIALIZATIONS  //

  template <> $::Shared::Pointer<Callable> cast(const Erased &type, bool strict);
  template <> $::Shared::Pointer<Prototype> cast(const Erased &type, bool strict);
};

} // namespace Sabre::Type

#endif
