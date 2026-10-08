#ifndef _SABRE_MEMBER_FACTORY_HPP
#define _SABRE_MEMBER_FACTORY_HPP

/// Sabre Includes
#include "sabre/member/reference.hpp"
#include "sabre/runtime/isolate.hpp"

/// Value Includes
#include "sabre/value/_inline/value.ipp"

namespace Sabre::Member::Factory {

/**
 * @brief Constructs a reference value.
 * @param args                  Arguments to bind.
 */
template <class... As> static inline $::Unique::Pointer<Reference> reference(As &&...args) {
  return $::Unique::New<Reference>(std::forward<As>(args)...);
}

/**
 * @brief Constructs a native descriptor.
 * @param isolate               Runtime isolate.
 * @param args                  Arguments to bind.
 */
template <class... As>
static inline $::Unique::Pointer<Reference> native(
    Runtime::Isolate *isolate, Function::Callback callback, const $::String::View &parent, const $::String::View &field
) {
  return reference(field, isolate->create<Function::Native>(callback, parent, field));
}

} // namespace Sabre::Member::Factory

#endif
