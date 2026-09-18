#ifndef _SABRE_OBJECT_GUARD_HPP
#define _SABRE_OBJECT_GUARD_HPP

/// Sabre Includes
#include "sabre/object/common.hpp"

namespace Sabre::Object {

/// @brief Object Lock-Guard.
class Guard {
  //  PROPERTIES  //

  /// @brief Encapsulated object header.
  Header *m_header;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an object guard.
   * @param value                     Value to guard.
   */
  explicit Guard(const Value::Any &value) :
      m_header(value.is<Object::Any>() ? value.m_as<Object::Any>()->header() : nullptr) {
    if (m_header) m_header->mutex().lock();
  }

  /// @brief Do not allow copy-construction.
  Guard(const Guard &) = delete;

  /// @brief Unlocks the object on destruction.
  ~Guard() {
    if (m_header) m_header->mutex().unlock();
  }

  //  OPERATOR METHODS  //

  /// @brief Do not allow copy-assignment.
  Guard &operator=(const Guard &) = delete;
};

} // namespace Sabre::Object

#endif
