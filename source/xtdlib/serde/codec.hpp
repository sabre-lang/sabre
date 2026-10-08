#ifndef _XTDLIB_SERDE_CODEC_HPP
#define _XTDLIB_SERDE_CODEC_HPP

/// Library Includes
#include "xtdlib/serde/value.hpp"
#include "xtdlib/utility/static.hpp"

namespace $::Serde {

/// @brief Allows Codec Introspection.
template <class T> class Inspect : private Ensure::Static, protected T {
  //  PROPERTIES  //

  /// @brief Denotes an encoder validation.
  static inline constexpr bool m_encodable = requires(const T &self) {
    { T::m_encode(self) } -> std::convertible_to<Value>;
  };

  /// @brief Denotes a decoder validation.
  static inline constexpr bool m_decodable = requires(const Value &value) { T::m_decode(value); };

public:
  //  PUBLIC METHODS  //

  /**
   * @brief Handles encoding values.
   * @param value               Value to encode.
   * @param args                Extra arguments.
   */
  template <class... As> static inline constexpr auto encode(const T &value, As &&...args) {
    if constexpr (Value::Constructible<T>) return Value(value);
    else return T::m_encode(value, std::forward<As>(args)...);
  }

  /**
   * @brief Handles decoding values.
   * @param value               Value to decode.
   * @param args                Extra arguments.
   */
  template <class... As> static inline constexpr auto decode(const Value &value, As &&...args) {
    if constexpr (Value::Trivial<T>) return *value.as<T>();
    else if constexpr (Value::Arithmetic<T>) return *value.as<T>(0);
    else return T::m_decode(value, std::forward<As>(args)...);
  }
};

//  PUBLIC METHODS  //

/**
 * @brief Handles encoding values.
 * @param value                 Value to encode.
 * @param args                Extra arguments.
 */
template <class T, class... As> $_INLINE_PERF auto Encode(const T &value, As &&...args) {
  return Inspect<T>::encode(value, std::forward<As>(args)...);
}

/**
 * @brief Handles encoding arrays.
 * @param value                 Array to encode.
 * @param args                Extra arguments.
 */
template <class T, class... As> $_INLINE_PERF auto Encode(const std::vector<T> &elements, As &&...args) {
  // prepare some details about the items
  auto length = elements.size();
  auto encoded = Array(length);

  // iterate over the elements to be used
  for (size_t ii = 0; ii < length; ++ii) encoded[ii] = Encode<T>(elements[ii], std::forward<As>(args)...);

  // resolve the encoded elements now
  return encoded;
}

/**
 * @brief Handles encoding objects.
 * @param value                 Object to encode.
 * @param args                Extra arguments.
 */
template <class T, class... As> $_INLINE_PERF auto Encode(const Map::Dict<T> &elements, As &&...args) {
  // prepare some details about the items
  auto encoded = Object();

  // iterate over the elements to be used
  for (const auto &[key, value] : elements) encoded[key] = Encode<T>(value, std::forward<As>(args)...);

  // resolve the encoded elements now
  return encoded;
}

/**
 * @brief Handles decoding values.
 * @param value                 Value to decode.
 * @param args                Extra arguments.
 */
template <class T, class... As> $_INLINE_PERF auto Decode(const Value &value, As &&...args) {
  return Inspect<T>::decode(value, std::forward<As>(args)...);
}

/**
 * @brief Handles decoding arrays.
 * @param value                 Array to decode.
 * @param args                Extra arguments.
 */
template <class T, class... As> $_INLINE_PERF auto Decode(const Array &elements, As &&...args) {
  // determine the expected return-typing
  using R = decltype(Decode<T>(Value()));

  // prepare some details about the items
  auto length = elements.size();
  auto decoded = std::vector<R>();
  decoded.reserve(length); // bind

  // iterate over the elements to be used
  for (size_t ii = 0; ii < length; ++ii) decoded.emplace_back(Decode<T>(elements[ii], std::forward<As>(args)...));

  // resolve the encoded elements now
  return decoded;
}

/**
 * @brief Handles decoding objects.
 * @param value                 Object to decode.
 * @param args                Extra arguments.
 */
template <class T, class... As> $_INLINE_PERF auto Decode(const Object &elements, As &&...args) {
  // determine the expected return-typing
  using R = decltype(Decode<T>(Value()));

  // prepare some details about the items
  auto decoded = Map::Dict<R>();

  // iterate over the elements to be used
  for (const auto &[key, value] : elements) decoded[key] = Decode<T>(value, std::forward<As>(args)...);

  // resolve the encoded elements now
  return decoded;
}

} // namespace $::Serde

#endif
