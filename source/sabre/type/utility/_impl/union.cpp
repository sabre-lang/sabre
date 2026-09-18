/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PRIVATE METHODS  //

Sabre::Type::Erased Sabre::Type::Union::m_infer(Constraints *constraints) const {
  // prepare the base union to be inherited
  Erased result = New::never();

  // iterate over the available children now
  for (const auto &element : m_elements) {
    result = New::merge(result, element->infer(constraints));
    if (result->is<Any>()) break; // stop early if possible
  }

  // return the merged typing now
  return result;
}

bool Sabre::Type::Union::m_unify(const Erased &candidate, Constraints *constraints) const {
  // attempt getting the other items children as necessary now
  auto elements = candidate->is<Union>() ? candidate->as<Union>()->m_elements : std::vector({candidate});

  // prepare a predicate to be used
  auto predicate = [this, constraints](const Erased &other) -> bool {
    auto check = [other, constraints](const Erased &element) { return element->unify(other, constraints); };
    return std::ranges::any_of(m_elements, check); // and attempt checking now via the reference check
  };

  // ensure our children fulfill the candidates children
  return std::ranges::all_of(elements, predicate);
}
