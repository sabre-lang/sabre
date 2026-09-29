#ifndef _XTDLIB_STRING_TRIM_HPP
#define _XTDLIB_STRING_TRIM_HPP

/// Library Includes
#include "xtdlib/string/view.hpp"

namespace $::Trim {

/// @brief Gets the whitespace details.
static inline consteval String::View whitespace() { return " \n\r\t\v\f"; }

/**
 * @brief Trims leading tokens.
 * @param view              View to trim.
 * @param blacklist         Trim characters.
 */
static inline constexpr String::View leading(const String::View &view, const String::View &blacklist = whitespace()) {
  auto start = view.find_first_not_of(blacklist); // prepare now
  return start == String::Term ? "" : view.substr(start);
}

/**
 * @brief Trims trailing tokens.
 * @param view              View to trim.
 * @param blacklist         Trim characters.
 */
static inline constexpr String::View trailing(const String::View &view, const String::View &blacklist = whitespace()) {
  return view.substr(0, view.find_last_not_of(blacklist) + 1);
}

/**
 * @brief Trims from both ends.
 * @param view              View to trim.
 * @param blacklist         Trim characters.
 */
static inline constexpr String::View both(const String::View &view, const String::View &blacklist = whitespace()) {
  auto start = view.find_first_not_of(blacklist);
  if (start == String::Term) return "";
  auto end = view.find_last_not_of(blacklist);
  return view.substr(start, end - start + 1);
}

/**
 * @brief Handles removing a prefix.
 * @param view              View to trim.
 * @param needle            Prefix needle.
 */
static inline constexpr String::View prefix(const String::View &view, const String::View &needle) {
  return view.starts_with(needle) ? view.substr(needle.size()) : view;
}

/**
 * @brief Handles removing a suffix.
 * @param view              View to trim.
 * @param needle            Suffix needle.
 */
static inline constexpr String::View suffix(const String::View &view, const String::View &needle) {
  return view.ends_with(needle) ? view.substr(0, view.size() - needle.size()) : view;
}

} // namespace $::Trim

#endif
