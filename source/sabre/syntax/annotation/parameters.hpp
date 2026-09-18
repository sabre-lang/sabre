#ifndef _SABRE_ANNOTATION_PARAMETERS_HPP
#define _SABRE_ANNOTATION_PARAMETERS_HPP

/// Sabre Includes
#include "sabre/syntax/declaration/variable.hpp"

namespace Sabre::Syntax {

/// @brief Signature Parameters Node.
struct Parameters : public Mixin<Parameters, Annotation> {
  //  TYPEDEFS  //

  /// @brief Encapsulates spread parameters.
  struct Spread;

  /// @brief Parameters List Typing.
  using List = std::vector<Variable *>;

protected:
  //  PROPERTIES  //

  /// @brief Associated parameters list.
  List m_list = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a set of parameters.
   * @param list                      Parameters list.
   */
  explicit Parameters(const List &list = {}) : m_list(list) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the available parameters.
  inline constexpr const List &list() const noexcept { return m_list; }

  /// @brief Gets the spread parameter.
  virtual inline constexpr const Variable *spread() const noexcept { return nullptr; }

  /// @brief Gets the minimum parameters sizing.
  virtual inline size_t arity() const noexcept {
    auto iter = std::ranges::find_if(m_list, [](Variable *variable) { return !variable->required(); });
    return iter == m_list.cend() ? m_list.size() : std::distance(m_list.cbegin(), iter); // resolve
  }

  /// @brief Gets the maximum parameters sizing.
  virtual inline size_t adicity() const noexcept { return m_list.size(); }
};

/// @brief Encapsulates Spread Parameters.
struct Parameters::Spread : public Parameters {
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a set of parameters.
   * @param list                      Parameters list.
   */
  explicit Spread(const List &list = {}) : Parameters(list) {
    $_ASSERT(list.size(), "Expected at least one-parameter when using spreads");
  }

  //  PUBLIC METHODS  //

  /// @brief Gets the spread parameter.
  inline constexpr const Variable *spread() const noexcept final { return m_list.back(); }

  /// @brief Gets the minimum parameters sizing.
  inline size_t arity() const noexcept final { return Parameters::arity() - 1; }

  /// @brief Gets the maximum parameters sizing.
  inline size_t adicity() const noexcept final { return SIZE_MAX; }
};

} // namespace Sabre::Syntax

#endif
