#ifndef _SABRE_TYPE_CALLABLE_HPP
#define _SABRE_TYPE_CALLABLE_HPP

/// Sabre Includes
#include "sabre/type/entity.hpp"

/// Type Includes
#include "sabre/type/utility/intrinsics.hpp"

namespace Sabre::Type {

/// @brief Callable Function Typing.
class Callable : public Mixin<Callable> {
  //  TYPEDEFS  //

  /// @brief Allow prototypes internal access.
  friend class Prototype;

  //  PROPERTIES  //

  /// @brief Attach parameter values.
  std::vector<Entity> m_parameters = {};

  /// @brief Expected return typing.
  Erased m_returns = New::none();

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a function hint.
   * @param returns               Return typing.
   */
  explicit Callable(const Erased &returns = New::any()) : m_returns(returns) {}

  /**
   * @brief Constructs a function hint.
   * @param returns               Return typing.
   * @param parameters            Parameter types.
   */
  explicit Callable(const Erased &returns, const std::vector<Entity> &parameters) :
      m_parameters(parameters), m_returns(returns) {}

  /**
   * @brief Constructs a function hint.
   * @param returns               Return typing.
   * @param parameters            Parameter types.
   */
  explicit Callable(const Erased &returns, const std::vector<Erased> &parameters) : m_returns(returns) {
    static constexpr auto predicate = [](const Erased &type) { return Entity(type); };
    m_parameters = $::Ranges::To(parameters | std::views::transform(predicate));
  }

  //  PROPERTIES  //

  /// @brief Denotes if the instance is packed.
  virtual inline constexpr bool packed() const noexcept { return false; }

  /// @brief Gets the bound parameters.
  inline constexpr std::vector<Entity> &parameters() noexcept { return m_parameters; }
  inline constexpr const std::vector<Entity> &parameters() const noexcept { return m_parameters; }

  /// @brief Gets the return typing.
  inline constexpr Erased &returns() noexcept { return m_returns; }
  inline constexpr const Erased &returns() const noexcept { return m_returns; }

  /// @brief Gets the truthiness of a callable.
  inline constexpr $::Unit::Ternary truthiness() const noexcept final { return true; }

  /// @brief Callables always validate to function instances.
  inline constexpr Shape::Underlying shape() const noexcept final { return Shape::Lookup<Function::Closure>(); }

  /// @brief Gets the minimum parameters available.
  inline constexpr size_t arity() const noexcept {
    for (const auto &[ii, entity] : $::Ranges::Each(m_parameters)) {
      if (packed() && &entity == &m_parameters.back()) break;
      if (entity.optional()) return ii; // check optionality
    }

    // return the resulting distance to be used now
    return m_parameters.size() - packed();
  }

  /// @brief Gets the maximum parameters available.
  inline constexpr size_t adicity() const noexcept { return packed() ? SIZE_MAX : m_parameters.size(); }

  /**
   * @brief Handles looking up callable fields.
   * @param field                 Field to lookup.
   */
  inline constexpr Entity lookup(const $::String::View &field) const final { return m_lookup(field); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles looking up callable fields.
   * @param field                 Field to lookup.
   */
  Entity m_lookup(const $::String::View &field) const;

  /**
   * @brief Handles inferring a function.
   * @param constraints               Generic constraints.
   */
  Erased m_infer(Constraints *constraints) const final;

  /**
   * @brief Handles running a unification pass.
   * @param candidate                 Candidate to unify.
   * @param constraints               Generic constraints.
   */
  bool m_unify(const Erased &candidate, Constraints *constraints) const final;

  /**
   * @brief Handles printing the type.
   * @param os                        Output stream.
   * @param self                      Callable instance.
   */
  static void m_print(std::ostream &os, const Callable &self);
};

/// @brief Variadic Function Typing.
struct Variadic : public Callable {
  //  CONSTRUCTORS  //

  /**
   * @brief Allow construction from other values.
   * @param other                     Callable value.
   */
  explicit Variadic(const Callable &other) : Callable(other) {}
  explicit Variadic(Callable &&other) : Callable(std::move(other)) {}

  /**
   * @brief Constructs a defaulted variadic function.
   * @param returns                   Return typing.
   */
  explicit Variadic(const Erased &returns = New::any()) : Variadic(returns, std::vector({New::any()})) {}

  /**
   * @brief Constructs a variadic function.
   * @param returns                   Return typing.
   * @param parameters                Parameters to bind.
   */
  explicit Variadic(const Erased &returns, const std::vector<Erased> &parameters) : Callable(returns, parameters) {}

  /**
   * @brief Constructs a variadic function.
   * @param returns                   Return typing.
   * @param parameters                Parameters to bind.
   */
  explicit Variadic(const Erased &returns, const std::vector<Entity> &parameters) : Callable(returns, parameters) {}

  //  PUBLIC METHODS  //

  /// @brief Denotes if the instance is packed.
  inline constexpr bool packed() const noexcept final { return true; }
};

} // namespace Sabre::Type

#endif
