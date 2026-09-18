#ifndef _SABRE_TYPE_UNION_HPP
#define _SABRE_TYPE_UNION_HPP

/// Type Includes
#include "sabre/type/utility/intrinsics.hpp"

namespace Sabre::Type {

/// @brief Union Typing.
class Union : public Mixin<Union> {
  //  PROPERTIES  //

  /// @brief Potential union types.
  std::vector<Erased> m_elements;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a union type.
   * @param elements                  Elements to hold.
   */
  explicit Union(const std::vector<Erased> &elements = {}) : m_elements(elements) {}

  //  PUBLIC METHODS  //

  /// @brief Gets a nodes associated truthiness.
  inline constexpr $::Unit::Ternary truthiness() const noexcept final {
    auto predicate = [](const auto &state, const auto &element) { return state && element->truthiness(); };
    return std::ranges::fold_left(m_elements, $::Unit::Ternary(false), predicate); // attempt resolving now
  }

  /// @brief Denotes if the union is empty.
  inline constexpr bool never() const noexcept { return m_elements.empty(); }

  /// @brief Gets the union elements.
  inline constexpr const std::vector<Erased> &elements() const noexcept { return m_elements; }

  /// @brief Gets the associated "Maybe" typing.
  inline constexpr Erased maybe() const noexcept {
    // ignore if we do not have the correct number of elements.
    if (m_elements.size() != 2) return nullptr;

    // pull out both elements to be queried
    auto first = m_elements.at(0), second = m_elements.at(1);

    // determine whether either is none or not
    auto fn = first->is<None>(), sn = second->is<None>();

    // and resolve to a suitable maybe element now
    return fn == sn ? nullptr : fn ? second : first;
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles type-instantiation.
   * @param constraints               Generic constraints.
   */
  Erased m_infer(Constraints *constraints) const final;

  /**
   * @brief Checks if candidate is a subtype of this type.
   * @param candidate                 Candidate to compare.
   * @param constraints               Generic constraints.
   */
  bool m_unify(const Erased &candidate, Constraints *constraints) const final;

  /**
   * @brief Handles printing the type.
   * @param os                        Output stream.
   * @param self                      Union instance.
   */
  static inline void m_print(std::ostream &os, const Union &self) {
    if (self.m_elements.empty()) os << "Never";
    else if (auto type = self.maybe()) os << *type << '?';
    else os << fmt::format("Union[{0}]", fmt::join($::Ranges::Deref(self.m_elements), ", "));
  }
};

} // namespace Sabre::Type

#endif
