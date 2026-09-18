#ifndef _SABRE_HANDLE_LOCAL_HPP
#define _SABRE_HANDLE_LOCAL_HPP

/// Sabre Includes
#include "sabre/value/void.hpp"

namespace Sabre::Handle {

/// @brief Handle Local Value.
template <std::derived_from<Value::Any> T> class Local {
  //  PROPERTIES  //

  /// @brief Tagged pointer value.
  Value::Any m_value = {};

  /// @brief Associated thread isolate.
  Runtime::Isolate *const m_isolate = nullptr;

  /// @brief Next local value in list.
  Local<Value::Any> *const m_next = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Do not allow default construction.
  explicit Local() = delete;

  /**
   * @brief Constructs a defaulted local.
   * @param scope                 Handle scope.
   */
  explicit Local(Scope *scope);

  /**
   * @brief Immediately scopes a value.
   * @param scope                 Handle scope.
   * @param value                 Local value.
   */
  explicit Local(Scope *scope, const Value::Any &value);

  /**
   * @brief Allow construction from derived handles.
   * @tparam U                    Derived type.
   * @param scope                 Handle scope.
   * @param other                 Local value.
   */
  template <std::derived_from<T> U> explicit Local(Scope *scope, const Local<U> &other) : Local(scope, other.m_value) {}

  /// @brief Handles removing local references (defined alongside "Runtime::Isolate").
  ~Local();

  //  OPERATOR METHODS  //

  /// @brief Allow access into the type.
  inline constexpr T *operator->() { return std::bit_cast<T *>(&m_value); }
  inline constexpr const T *operator->() const { return std::bit_cast<const T *>(&m_value); }

  /// @brief Allow casting to the base value.
  inline constexpr T &operator*() const { return *std::bit_cast<T *>(&m_value); }

  /// @brief Allow down-casting when T is a subset of U.
  template <std::derived_from<Value::Any> U> inline constexpr operator const Local<U> &() const {
    static_assert(std::derived_from<U, T>); // sanity
    return *reinterpret_cast<const Local<U> *>(this);
  }

  //  PUBLIC METHODS  //

  /// @brief Gets the next local handle.
  inline constexpr Local<Value::Any> *next() noexcept { return m_next; }
  inline constexpr const Local<Value::Any> *next() const noexcept { return m_next; }

  /// @brief Gets the bound reference value.
  inline constexpr Value::Any &value() noexcept { return m_value; }
  inline constexpr Value::Any value() const noexcept { return m_value; }

protected:
  //  PRIVATE METHODS  //

  /// @brief Gets a relative reference to the local.
  inline Local<Value::Any> *m_reference() { return reinterpret_cast<Local<Value::Any> *>(this); }

  /**
   * @brief Handles printing raw values.
   * @param os                    Output stream.
   * @param self                  Local instance.
   */
  static inline void m_print(std::ostream &os, const Local &self) { os << self.m_value; }
};

} // namespace Sabre::Handle

#endif
