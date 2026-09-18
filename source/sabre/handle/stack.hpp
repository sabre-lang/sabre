#ifndef _SABRE_HANDLE_STACK_HPP
#define _SABRE_HANDLE_STACK_HPP

/// Sabre Includes
#include "sabre/forward/handle.hpp"
#include "sabre/forward/value.hpp"

namespace Sabre::Handle {

/// @brief Linked Stack of Handles.
class Stack {
  //  PROPERTIES  //

  /// @brief Top-most value.
  Handle::Local<Value::Any> *m_head = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Default constructor.
  constexpr Stack() = default;

  //  PUBLIC METHODS  //

  /// @brief Current head of list.
  inline Handle::Local<Value::Any> *head() const { return m_head; }

  /**
   * @brief Pops the top-most value.
   * @param previous          Previous local value.
   */
  inline void pop(Handle::Local<Value::Any> *previous = nullptr) { m_head = previous; }

  /**
   * @brief Pushes a handle onto the list.
   * @param next              Next local value.
   */
  inline Handle::Local<Value::Any> *push(Handle::Local<Value::Any> *next) {
    auto *previous = m_head;
    m_head = next;
    return previous;
  }
};

} // namespace Sabre::Handle

#endif
